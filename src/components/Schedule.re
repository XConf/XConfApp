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
            backgroundColor(Theme.Color.background),
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
            backgroundColor(Theme.Color.lightGrey),
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

  type listItem('a) =
    | StartDate(Js.Date.t)
    | Item('a);

  let periodSectionedListOfSchedule = schedule => {
    let sortingDict = sortingDictFromPlaces(schedule##places);

    schedule##items
    |> Array.to_list
    |> group(
         ~same=(a, b) => DateFns.isEqual(a, b),
         a => (a##periods)[0]##start,
       )
    |> List.sort(((startA, _), (startB, _)) =>
         DateFns.compareAsc(startA, startB)
       )
    |> List.map(((startDate, items)) =>
         (
           startDate,
           List.sort(
             compareScheduleItemsWithSortingDict(sortingDict),
             items,
           ),
         )
       )
    |> List.map(((startDate, items)) =>
         [StartDate(startDate), ...List.map(i => Item(i), items)]
       )
    |> List.flatten;
  };
};

let make = (~schedule, ~onItemPress, ~onRefresh, ~refreshing, _children) => {
  ...component,
  initialState: () => {scrollViewRef: ref(None)},
  reducer: (_action: action, _state: state) => ReasonReact.NoUpdate,
  render: self => {
    let periodSectionedList =
      DataProcessor.periodSectionedListOfSchedule(schedule);

    let elements =
      DataProcessor.(
        periodSectionedList
        |> List.map(
             fun
             | StartDate(startDate) => [
                 <ScheduleTimeLabel time=startDate />,
                 <View style=styles##afterTimeLabel />,
               ]
             | Item(item) => [
                 <View style=styles##cellContainer>
                   <ScheduleItemCell scheduleItem=item onPress=onItemPress />
                 </View>,
               ]
           )
        |> List.flatten
        |> Array.of_list
      );

    let labelIndexes =
      DataProcessor.(
        periodSectionedList
        |> List.map(
             fun
             | StartDate(_) => [true, false]
             | _ => [false],
           )
        |> List.flatten
        |> List.mapi((i, isDate) => (i, isDate))
        |> filter(((_, isDate)) => isDate)
        |> List.map(((i, _)) => i)
      );
    <ScrollView
      ref=(self.handle(setScrollViewRef))
      style=styles##scrollView
      contentContainerStyle=styles##scrollViewContent
      refreshControl={<RefreshControl refreshing onRefresh />}
      stickyHeaderIndices=labelIndexes>
      ...(Array.append(elements, [|<View style=styles##timeLine />|]))
    </ScrollView>;
  },
};
