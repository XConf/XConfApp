let component = ReasonReact.statelessComponent("ConferenceTimeAndPlace");

type conference = {. "name": string};

let styles =
  StyleSheet.create(
    Style.(
      {
        "content":
          style([
            flex(1.),
            paddingVertical(Pt(128.)),
            paddingHorizontal(Pt(16.)),
            justifyContent(Center),
            alignItems(Center),
          ]),
      }
    ),
  );

let make = (~conference: conference, _children) => {
  ...component,
  render: _self =>
    <View style=styles##content> <Text value=conference##name /> </View>,
};
