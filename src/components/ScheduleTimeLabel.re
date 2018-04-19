let component = ReasonReact.statelessComponent("ScheduleTimeLabel");

let ptHeight = 28.;
let ptWidth = 84.;

let styles =
  StyleSheet.create(
    Style.(
      {
        "container":
          style([
            top(Pt(0.)),
            left(Pt(0.)),
            height(Pt(ptHeight)),
            width(Pt(ptWidth)),
            justifyContent(Center),
            alignItems(Center),
          ]),
        "text":
          style([
            color(Theme.Color.darkerGrey),
            fontSize(Float(12.)),
            fontFamily("Roboto Mono"),
            fontWeight(`_300)
          ]),
      }
    ),
  );

let make = (~time: Js.Date.t, _children) => {
  ...component,
  render: _self =>
    <View pointerEvents=`none style=styles##container>
      <Text style=styles##text value=(DateFns.format("h:mm a", time)) />
    </View>,
};
