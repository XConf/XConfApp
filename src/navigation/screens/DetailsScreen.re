type state = {
  editMode: bool
};

type action =
  | ToggleEditMode;

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
  initialState: () => {editMode: false},
  reducer: (action, state) =>
    switch action {
    | ToggleEditMode => ReasonReact.Update({...state, editMode: !state.editMode})
    },
  render: (self) => {
    let title = switch (self.state.editMode) {
      | true => "Editing Details"
      | false => "Details"
    };

    let buttonTitle = switch (self.state.editMode) {
      | true => "Done"
      | false => "Edit"
    };

    <View style=styles##container>
      <HeaderTitleText value=title />
      <HeaderRight>
        <Button title=buttonTitle onPress=((_event) => self.send(ToggleEditMode)) />
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
