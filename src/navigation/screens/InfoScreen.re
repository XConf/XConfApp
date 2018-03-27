let component = ReasonReact.statelessComponent("InfoScreen");

let styles =
  BsReactNative.(
    StyleSheet.create(
      Style.(
        {
          "container":
            style([
              flex(1.),
              paddingVertical(Pt(128.)),
              paddingHorizontal(Pt(16.)),
              justifyContent(Center),
              alignItems(Center)
            ])
        }
      )
    )
  );

let make = (~onClosePress: unit => unit, ~onOpenAnotherModalPress: unit => unit, _children) => {
  ...component,
  render: (_self) =>
    <View style=styles##container>
      <Text value="Hi, this is some info for you." />
      /*<Button title="Open Another Modal" onPress=onOpenAnotherModalPress />*/
      <Button title="Close" onPress=onClosePress />
    </View>
};
