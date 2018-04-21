let component = ReasonReact.statelessComponent("Tag");

type value('a) =
  | Language('a)
  | Other(string);

let styles =
  StyleSheet.create(
    Style.(
      {
        "tag":
          style([
            marginVertical(Pt(4.)),
            marginRight(Pt(6.)),
            height(Pt(20.)),
            borderRadius(3.),
            paddingHorizontal(Pt(6.)),
            justifyContent(Center),
            alignItems(Center),
            backgroundColor(Theme.Color.lighterGrey),
          ]),
        "tagText":
          style([
            fontFamily("Roboto"),
            fontWeight(`_200),
            fontSize(Float(12.)),
            color(Theme.Color.darkGrey),
          ]),
        "rubyTag":
          style([
            marginVertical(Pt(4.)),
            marginRight(Pt(6.)),
            height(Pt(20.)),
            borderRadius(3.),
            paddingHorizontal(Pt(6.)),
            justifyContent(Center),
            alignItems(Center),
            backgroundColor("#fff2f0"),
          ]),
        "rubyTagText":
          style([
            fontFamily("Roboto"),
            fontWeight(`_200),
            fontSize(Float(12.)),
            color("#eb6047"),
          ]),
        "elixirTag":
          style([
            marginVertical(Pt(4.)),
            marginRight(Pt(6.)),
            height(Pt(20.)),
            borderRadius(3.),
            paddingHorizontal(Pt(6.)),
            justifyContent(Center),
            alignItems(Center),
            backgroundColor("#f8f0ff"),
          ]),
        "elixirTagText":
          style([
            fontFamily("Roboto"),
            fontWeight(`_200),
            fontSize(Float(12.)),
            color("#9a72c0"),
          ]),
      }
    ),
  );

let make = (~value: 'a, _children) => {
  ...component,
  render: _self =>
    switch (value) {
    | Language(`EN) =>
      <View style=styles##tag>
        <Text style=styles##tagText value="EN" />
      </View>
    | Language(`ZH_TW) =>
      <View style=styles##tag>
        <Text style=styles##tagText value="CHT" />
      </View>
    | Other("Ruby") =>
      <View style=styles##rubyTag>
        <Text style=styles##rubyTagText value="Ruby" />
      </View>
    | Other("Elixir") =>
      <View style=styles##elixirTag>
        <Text style=styles##elixirTagText value="Elixir" />
      </View>
    | Other(name) =>
      <View style=styles##tag>
        <Text style=styles##tagText value=name />
      </View>
    },
};
