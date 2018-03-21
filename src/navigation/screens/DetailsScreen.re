type state = {
  editMode: bool,
  headerStyle: string
};

type action =
  | ToggleEditMode
  | ChangeHeaderStyle(string);

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
  initialState: () => {editMode: false, headerStyle: "light"},
  reducer: (action, state) =>
    switch action {
    | ToggleEditMode => ReasonReact.Update({...state, editMode: !state.editMode})
    | ChangeHeaderStyle(style) => ReasonReact.Update({...state, headerStyle: style})
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

    let modeMessage = switch (self.state.editMode) {
      | true => " [Edit Mode]"
      | false => ""
    };

    let handleGoToAnotherDetailsScreenButtonPress = () =>
      onGoToAnotherDetailsPress(string_of_int(Random.bits()));

    <View style=styles##container>
      <StackNavigatorHeader style=self.state.headerStyle>
        <StackNavigatorHeader.TitleText value=title />
        <StackNavigatorHeader.HeaderRight>
          <Button title=buttonTitle onPress=((_event) => self.send(ToggleEditMode)) />
        </StackNavigatorHeader.HeaderRight>
      </StackNavigatorHeader>
      <Text value={j|Details Screen$modeMessage|j} />
      <Text />
      <Text value={j|Param: $param|j} />
      <Text />
      <Button
        title="Change to Default Style"
        onPress=((_event) => self.send(ChangeHeaderStyle("default")))
      />
      <Button
        title="Change to Light Style"
        onPress=((_event) => self.send(ChangeHeaderStyle("light")))
      />
      <Button
        title="Go To Another Details Screen"
        onPress=handleGoToAnotherDetailsScreenButtonPress
      />
      <Button title="Go Back" onPress=onGoBackPress />
    </View>;
  }
};
