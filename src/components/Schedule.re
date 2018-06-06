type state = {scrollViewRef: ref(option(ReasonReact.reactRef))};

type action =
  | NoAction;

let component = ReasonReact.reducerComponent("Schedule");

let setScrollViewRef = (r, {ReasonReact.state}) =>
  state.scrollViewRef := Js.Nullable.toOption(r);

let scrollScrollViewToTop = (_, {ReasonReact.state}) =>
  switch (state.scrollViewRef^) {
  | None => ()
  | Some(r) => ScrollView.scrollTo(r, ~x=0, ~y=0, ~animated=true)
  };

let scrollToTop = r => {
  let self = ReasonReactUtils.refToSelf(r);
  self.handle(scrollScrollViewToTop, ());
};

let styles =
  StyleSheet.create(
    Style.(
      {
        "scrollView":
          style([
            flex(1.),
            paddingVertical(Pt(4.)),
            backgroundColor(String(Theme.Color.background)),
          ]),
        "scrollViewContent":
          style([
            paddingBottom(Pt(16.)),
          ]),
        "content":
          style([
            flex(1.),
          ]),
        "afterTimeLabel":
          style([
            marginTop(Pt(-. ScheduleTimeLabel.ptHeight))
          ]),
        "cellContainer": style([
          zIndex(20),
          marginLeft(Pt(ScheduleTimeLabel.ptWidth))
        ]),
        "timeLine":
          style([
            position(Absolute),
            top(Pt(14.)),
            left(Pt(ScheduleTimeLabel.ptWidth +. 3.5)),
            width(Pt(1.)),
            height(Pt(10000.)),
            backgroundColor(String(Theme.Color.lightGrey)),
          ]),
      }
    ),
  );

module DataProcessor = {
  let group = (~same=(a, b) => a == b, f: 'a => 'b, l: list('a)) => {
    let rec grouping = (acc, list) =>
      switch (list) {
      | [] => acc
      | [hd, ...tl] =>
        let c = f(hd);
        let (l1, l2) = List.partition(i => same(f(i), c), tl);
        grouping([(c, [hd, ...l1]), ...acc], l2);
      };
    grouping([], l);
  };

  let rec filter = (f, l) =>
    switch (l) {
    | [] => []
    | [hd, ...tl] when f(hd) => [hd, ...filter(f, tl)]
    | [_, ...tl] => filter(f, tl)
  };

  let sortingDictFromPlaces = [%bs.raw {|
    places => places.reduce((o, { id }, i) => { o[id] = i; return o; }, {})
  |}];

  let compareScheduleItemsWithSortingDict = [%bs.raw {|
    (sortingDict, itemA, itemB) => sortingDict[itemA.places[0].id] > sortingDict[itemB.places[0].id] ? 1 : -1
  |}];

  type listItem('a, 'b) =
    | OptionPeriod('a)
    | Item('b);

  let periodItemsListOfSchedule = schedule => {
    let sortingDict = sortingDictFromPlaces(schedule##places);

    schedule##items
    |> Array.to_list
    |> group(
         ~same=(a, b) => switch (a, b) {
         | (Some(dateA), Some(dateB)) => dateA##id === dateB##id
         | _ => false
         },
         a => ArrayUtils.safeGet(a##periods, 0),
       )
    |> List.sort(((optionDateA, _), (optionDateB, _)) =>
        switch (optionDateA, optionDateB) {
        | (Some(dateA), Some(dateB)) => DateFns.compareAsc(dateA##start, dateB##start)
        | (None, Some(_)) => 1
        | (Some(_), None) => -1
        | _ => 0
        }
       )
    |> List.map(((period, items)) =>
         (
           period,
           List.sort(
             compareScheduleItemsWithSortingDict(sortingDict),
             items,
           ),
         )
       );
  };

  let flattenPeriodItemsList = periodItemsList => {
    periodItemsList
    |> List.map(((period, items)) =>
         [OptionPeriod(period), ...List.map(i => Item(i), items)]
       )
    |> List.flatten;
  };
};

let make = (~schedule, ~onItemPress, ~onRefresh, ~refreshing, _children) => {
  ...component,
  initialState: () => {scrollViewRef: ref(None)},
  reducer: (_action: action, _state: state) => ReasonReact.NoUpdate,
  render: self => {
    let periodItemsList =
      DataProcessor.periodItemsListOfSchedule(schedule);

    let dateGroupedPeriodItemsList =
      DataProcessor.(
        periodItemsList
        |> group(((optionPeriod, _)) => switch (optionPeriod) {
        | Some(period) => Some(period##date)
        | None => None
        })
        |> List.sort(((optionDateA, _), (optionDateB, _)) =>
            switch (optionDateA, optionDateB) {
            | (Some(dateA), Some(dateB)) => DateFns.compareAsc(dateA##date, dateB##date)
            | (None, Some(_)) => 1
            | (Some(_), None) => -1
            | _ => 0
            }
           )
      );

    let elementsOfFlattenedStartDateItemsList = (flattenedPeriodItemsList) =>
      DataProcessor.(
        flattenedPeriodItemsList
        |> List.map(
             fun
             | OptionPeriod(Some(period)) => [
                 <ScheduleTimeLabel time=period##start />,
                 <View style=styles##afterTimeLabel />,
               ]
             | OptionPeriod(None) => []
             | Item(item) => [
                 <View style=styles##cellContainer>
                   <ScheduleItemCell scheduleItem=item onPress=onItemPress />
                 </View>,
               ]
           )
        |> List.flatten
        |> Array.of_list
      );

    let labelIndexesOfFlattenedStartDateItemsList = (flattenedPeriodItemsList) =>
      DataProcessor.(
        flattenedPeriodItemsList
        |> List.map(
             fun
             | OptionPeriod(_) => [true, false]
             | _ => [false],
           )
        |> List.flatten
        |> List.mapi((i, isPeriod) => (i, isPeriod))
        |> filter(((_, isPeriod)) => isPeriod)
        |> List.map(((i, _)) => i)
      );

    let tabs = dateGroupedPeriodItemsList
      |> List.map(((optionDate, periodItemsList)) => {
        let flattenedPeriodItemsList = DataProcessor.flattenPeriodItemsList(periodItemsList);
        let elements = elementsOfFlattenedStartDateItemsList(flattenedPeriodItemsList);
        let labelIndexes = labelIndexesOfFlattenedStartDateItemsList(flattenedPeriodItemsList);

        <SwipeableTabView.Tab
          title=(switch (optionDate) {
          | None => "General"
          | Some(date) => date##name
          })
        >
          <ScrollView
            ref=(self.handle(setScrollViewRef))
            style=styles##scrollView
            contentContainerStyle=styles##scrollViewContent
            refreshControl={<RefreshControl refreshing onRefresh />}
            stickyHeaderIndices=labelIndexes>
            ...(Array.append(elements, [|<View style=styles##timeLine />|]))
          </ScrollView>
        </SwipeableTabView.Tab>
      })
      |> Array.of_list;

    <SwipeableTabView>
      ...tabs
    </SwipeableTabView>;
  },
};
