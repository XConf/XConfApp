let component = ReasonReact.statelessComponent("MapScreen");

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

let make = (~onClosePress: unit => unit, _children) => {
  ...component,
  render: (_self) =>
    <View style=styles##container>
      <Text value="Hi, this is a map." />
      <Button title="Close" onPress=onClosePress />
    </View>
};
