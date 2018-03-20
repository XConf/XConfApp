type state = {
  overrideTitle: bool,
  title: string
};

type action =
  | OverrideTitle(string);

let component = ReasonReact.reducerComponent("DetailsScreen");

let styles =
  BsReactNative.(
    StyleSheet.create(
      Style.({"container": style([
        flex(1.),
        justifyContent(Center),
        alignItems(Center)
      ])})
    )
  );

let make =
    (~param, ~onGoBackPress: unit => unit, ~onGoToAnotherDetailsPress: string => unit, _children) => {
  ...component,
  initialState: () => {overrideTitle: false, title: ""},
  reducer: (action, state) =>
    switch action {
    | OverrideTitle(t) => ReasonReact.Update({...state, overrideTitle: true, title: t})
    },
  render: (self) => {
    let title = switch (self.state.overrideTitle) {
      | true => self.state.title
      | false => "Details"
    };

    let button = switch (self.state.overrideTitle) {
      | true => ReasonReact.nullElement
      | false => <Button title="Change" onPress=((_event) => self.send(OverrideTitle("Updated!"))) />
    };

    <View style=styles##container>
      <HeaderTitleText value=title />
      <HeaderRight>
        {button}
      </HeaderRight>
      <Text value="Details Screen" />
      <Text />
      <Text value={j|Param: $param|j} />
      <Text />
      <Button
        title="Go To Another Details Screen"
        onPress=(() => onGoToAnotherDetailsPress(string_of_int(Random.bits())))
      />
      <Button title="Go Back" onPress=onGoBackPress />
    </View>;
  }
};
