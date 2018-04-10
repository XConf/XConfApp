let component = ReasonReact.statelessComponent("ConferenceTimeAndPlace");

type conference('a) = {.. "name": string} as 'a;

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

let make = (~conference: conference('a), _children) => {
  ...component,
  render: _self =>
    <View style=styles##content> <Text value=conference##name /> </View>,
};
