let component = ReasonReact.statelessComponent("ScheduleItem");

/*type conference('a) = {.. "name": string} as 'a;*/
let styles =
  StyleSheet.create(
    Style.(
      {
        "content":
          style([
            paddingVertical(Pt(8.)),
            paddingHorizontal(Pt(16.)),
          ]),
      }
    ),
  );

let make = (~scheduleItem, ~onPress, _children) => {
  ...component,
  render: _self =>
    <TouchableOpacity style=styles##content onPress=(() => onPress(scheduleItem))>
      <Text value=scheduleItem##event##title />
    </TouchableOpacity>,
};
