let component = ReasonReact.statelessComponent("Tag");

let styles =
  StyleSheet.create(
    Style.(
      {
        "tag":
          style([
            marginVertical(Pt(4.)),
            marginRight(Pt(4.)),
            height(Pt(20.)),
            borderRadius(3.),
            paddingHorizontal(Pt(6.)),
            backgroundColor(Theme.Color.lighterGrey),
            justifyContent(Center),
            alignItems(Center),
          ]),
        "tagText":
          style([
            fontFamily("Roboto"),
            fontWeight(`_200),
            fontSize(Float(12.)),
            color(Theme.Color.darkGrey),
          ]),
      }
    ),
  );

let make = (~value: 'a, _children) => {
  ...component,
  render: _self =>
    switch (value) {
    | `EN =>
      <View style=styles##tag>
        <Text style=styles##tagText value="EN" />
      </View>
    | `ZH_TW =>
      <View style=styles##tag>
        <Text style=styles##tagText value={j|中文|j} />
      </View>
    },
};
