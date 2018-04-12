type state = {scrollViewRef: ref(option(ReasonReact.reactRef))};

type action =
  | NoAction;

let component = ReasonReact.reducerComponent("ScheduleItemDetails");

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

let styles = StyleSheet.create(Style.({"scrollView": style([flex(1.)])}));

let make = (~scheduleItem, ~onRefresh, ~refreshing, _children) => {
  ...component,
  initialState: () => {scrollViewRef: ref(None)},
  reducer: (_action: action, _state) => ReasonReact.NoUpdate,
  render: self =>
    <ScrollView
      ref=(self.handle(setScrollViewRef))
      style=styles##scrollView
      refreshControl={<RefreshControl refreshing onRefresh />}>
      (
        switch (scheduleItem##eventInterface) {
        | `Activity(activity) =>
          <View>
            <Text value="Activity:" />
            <Text value=activity##title />
          </View>
        | `Session(session) =>
          <View>
            <Text value="Session:" />
            <Text value=session##title />
            <Text value=session##speaker##name />
            <Text value=session##description />
          </View>
        }
      )
    </ScrollView>,
};
