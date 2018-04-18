let component = ReasonReact.statelessComponent("ScheduleItemCell");

/*type conference('a) = {.. "name": string} as 'a;*/
let styles =
  StyleSheet.create(
    Style.(
      {
        "container":
          style([
            flexDirection(Row)
          ]),
        "timeLine":
          style([
            top(Pt(14.)),
            left(Pt(3.5)),
            marginBottom(Pt(-14.)),
            width(Pt(1.)),
            backgroundColor("#d1d7db")
          ]),
        "timeLineDot":
          style([
            top(Pt(11.)),
            marginRight(Pt(-6.)),
            marginBottom(Pt(-6.)),
            width(Pt(6.)),
            height(Pt(6.)),
            borderWidth(1.),
            borderColor("#d1d7db"),
            backgroundColor("#d1d7db"),
            borderRadius(100.),
          ]),
        "content":
          style([
            flex(1.),
            paddingVertical(Pt(4.)),
            paddingHorizontal(Pt(16.)),
          ]),
        "titleText":
          style([
            fontSize(Float(16.)),
            fontFamily("Roboto"),
            fontWeight(`_500),
            paddingBottom(Pt(2.)),
          ]),
      }
    ),
  );

let make = (~scheduleItem, ~onPress, _children) => {
  ...component,
  render: _self =>
    <View style=styles##container>
      <View style=styles##timeLine />
      <View style=styles##timeLineDot />
      <TouchableOpacity style=styles##content onPress=(() => onPress(scheduleItem))>
        <Text style=styles##titleText value=scheduleItem##event##title />
        <Text value="Place" />
      </TouchableOpacity>
    </View>,
};
