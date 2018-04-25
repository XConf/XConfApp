type state = {componentRef: ref(option(ReasonReact.reactRef))};

type action =
  | NoAction;

let component = ReasonReact.reducerComponent("InformationScreen");

let setComponentRef = (r, {ReasonReact.state}) =>
  state.componentRef := Js.Nullable.toOption(r);

let componentScrollToTop = (_, {ReasonReact.state}) =>
  switch (state.componentRef^) {
  | None => ()
  | Some(r) => ConferenceInformation.scrollToTop(r)
  };

let scrollToTop = r => {
  let self = ReasonReactUtils.refToSelf(r);
  self.handle(componentScrollToTop, ());
};

let make = _children => {
  ...component,
  initialState: () => {componentRef: ref(None)},
  reducer: (_action: action, _state) => ReasonReact.NoUpdate,
  render: self =>
    <View style=Style.(style([flex(1.)]))>
      <StackNavigator.Header style="default">
        <StackNavigator.Header.TitleText value="Information" />
      </StackNavigator.Header>
      <ConferenceInformationContainer
        conferenceCode="2018"
        componentRef=(self.handle(setComponentRef))
      />
    </View>,
};
