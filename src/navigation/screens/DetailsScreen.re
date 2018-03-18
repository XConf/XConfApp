let component = ReasonReact.statelessComponent("DetailsScreen");

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
  render: (_self) =>
    <View style=styles##container>
      <HeaderTitleText value="Details" />
      <Text value="Details Screen" />
      <Text />
      <Text value={j|Param: $param|j} />
      <Text />
      <Button
        title="Go To Another Details Screen"
        onPress=(() => onGoToAnotherDetailsPress(string_of_int(Random.bits())))
      />
      <Button title="Go Back" onPress=onGoBackPress />
    </View>
};
