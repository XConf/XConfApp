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
            backgroundColor(Theme.Color.lightGrey),
          ]),
        "timeLineDot":
          style([
            top(Pt(11.)),
            marginRight(Pt(-6.)),
            marginBottom(Pt(-6.)),
            width(Pt(6.)),
            height(Pt(6.)),
            borderWidth(1.),
            borderColor(Theme.Color.lightGrey),
            backgroundColor(Theme.Color.lightGrey),
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
            paddingBottom(Pt(2.)),
            fontSize(Float(16.)),
            fontFamily("Roboto"),
            fontWeight(`_400),
            color(Theme.Color.black),
          ]),
        "text":
          style([
            paddingBottom(Pt(2.)),
            fontSize(Float(14.)),
            fontFamily("Roboto"),
            fontWeight(`_300),
            color(Theme.Color.darkerGrey),
          ]),
        "sideText":
          style([
            paddingBottom(Pt(2.)),
            fontSize(Float(14.)),
            fontFamily("Roboto"),
            fontWeight(`_300),
            color(Theme.Color.darkGrey),
          ]),
      }
    ),
  );

let rec insertSeparates = (separator: 'a, list: list('a)) => {
  switch (list) {
  | [] => []
  | [head] => [head]
  | [head, ...tail] => [head, separator, ...insertSeparates(separator, tail)]
  }
};

let make = (~scheduleItem, ~onPress, _children) => {
  ...component,
  render: _self =>
    <View style=styles##container>
      <View style=styles##timeLine />
      <View style=styles##timeLineDot />
      <TouchableOpacity style=styles##content onPress=(() => onPress(scheduleItem))>
        <Text style=styles##titleText value=scheduleItem##event##title />
        <Text>
          ...(
            scheduleItem##places
            |> Array.to_list
            |> List.map((places) => <Text style=styles##text value=places##name />)
            |> insertSeparates(<Text style=styles##sideText value={j| · |j} />)
            |> Array.of_list
          )
        </Text>
        <Text style=styles##text value=DateFns.distanceInWords(
          (scheduleItem##periods)[0]##start,
          (scheduleItem##periods)[Array.length(scheduleItem##periods) - 1]##_end,
        ) />
        /*<Text style=styles##text value="Place" />*/
      </TouchableOpacity>
    </View>,
};
