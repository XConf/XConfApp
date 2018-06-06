type state = {componentRef: ref(option(ReasonReact.reactRef))};

type action =
  | NoAction;

type scheduleItem = ConferenceScheduleContainer.scheduleItem;

let component = ReasonReact.reducerComponent("ScheduleScreen");

let setComponentRef = (r, {ReasonReact.state}) =>
  state.componentRef := Js.Nullable.toOption(r);

let componentScrollToTop = (_, {ReasonReact.state}) =>
  switch (state.componentRef^) {
  | None => ()
  | Some(r) => Schedule.scrollToTop(r)
  };

let scrollToTop = r => {
  let self = ReasonReactUtils.refToSelf(r);
  self.handle(componentScrollToTop, ());
};

let make = (~onMapPress: unit => unit, ~onScheduleItemPress, _children) => {
  ...component,
  initialState: () => {componentRef: ref(None)},
  reducer: (_action: action, _state: state) => ReasonReact.NoUpdate,
  render: self =>
    <View style=Style.(style([flex(1.), marginTop(Pt(-8.))]))>
      <StackNavigator.Header style="default-nops">
        /*<StackNavigator.Header.Left>
          <Button title="Map" onPress=onMapPress />
        </StackNavigator.Header.Left>*/
        <StackNavigator.Header.TitleText value="Schedule" />
      </StackNavigator.Header>
      <ConferenceScheduleContainer
        conferenceCode="2016"
        componentRef=(self.handle(setComponentRef))
        onScheduleItemPress
      />
    </View>,
};
