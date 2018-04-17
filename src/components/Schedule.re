type state = {scrollViewRef: ref(option(ReasonReact.reactRef))};

type action =
  | NoAction;

let component = ReasonReact.reducerComponent("Schedule");

let setScrollViewRef = (r, {ReasonReact.state}) =>
  state.scrollViewRef := Js.Nullable.toOption(r);

let scrollScrollViewToTop = (_, {ReasonReact.state}) =>
  switch (state.scrollViewRef^) {
  | None => ()
  | Some(r) => SectionScrollView.scrollToTop(r)
  };

let scrollToTop = r => {
  let self = ReasonReactUtils.refToSelf(r);
  self.handle(scrollScrollViewToTop, ());
};

let styles = StyleSheet.create(Style.({"scrollView": style([flex(1.)])}));

let group = (f: 'a => 'b, l: list('a)) => {
  let rec grouping = (acc, list) =>
    switch (list) {
    | [] => acc
    | [hd, ...tl] =>
      let c = f(hd);
      let (l1, l2) = List.partition(i => f(i) === c, tl);
      grouping([(c, [hd, ...l1]), ...acc], l2);
    };
  grouping([], l);
};

let sortingDictFromPlaces = [%bs.raw {|
  places => places.reduce((o, { id }, i) => { o[id] = i; return o; }, {})
|}];

let compareScheduleItemsWithSortingDict = [%bs.raw {|
  (sortingDict, itemA, itemB) => sortingDict[itemA.places[0].id] > sortingDict[itemB.places[0].id] ? 1 : -1
|}];

let make = (~schedule, ~onItemPress, ~onRefresh, ~refreshing, _children) => {
  ...component,
  initialState: () => {scrollViewRef: ref(None)},
  reducer: (_action: action, _state: state) => ReasonReact.NoUpdate,
  render: self => {
    let sortingDict = sortingDictFromPlaces(schedule##places);
    <SectionScrollView
      ref=(self.handle(setScrollViewRef))
      style=styles##scrollView
      refreshControl={<RefreshControl refreshing onRefresh />}>
      ...(
           schedule##items
           |> Array.to_list
           |> group(a => (a##periods)[0]##start)
           |> List.sort(((startA, _), (startB, _)) => DateFns.compareAsc(startA, startB))
           |> List.map(((start, items)) =>
                (
                  start,
                  List.sort(
                    compareScheduleItemsWithSortingDict(sortingDict),
                    items,
                  ),
                )
              )
           |> Array.of_list
           |> Array.map(((start, list)) => (start, Array.of_list(list)))
           |> Array.map(((start, items)) =>
                (
                  start,
                  Array.map(
                    item =>
                      <ScheduleItem
                        key=item##id
                        scheduleItem=item
                        onPress=onItemPress
                      />,
                    items,
                  ),
                )
              )
           |> Array.map(((start, elems)) =>
                <SectionScrollView.Section
                  header={<Text value=(DateFns.format("h:mm a", start)) />}>
                  ...elems
                </SectionScrollView.Section>
              )
         )
    </SectionScrollView>;
  },
};
