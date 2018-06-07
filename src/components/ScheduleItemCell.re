let component = ReasonReact.statelessComponent("ScheduleItemCell");

[@bs.get] external getStart : 'a => Js.Date.t = "start";
[@bs.get] external getEnd : 'a => Js.Date.t = "end";

let styles =
  StyleSheet.create(
    Style.(
      {
        "container":
          style([
            flexDirection(Row),
          ]),
        "timeLine":
          style([
            top(Pt(14.)),
            left(Pt(3.5)),
            marginBottom(Pt(-14.)),
            width(Pt(1.)),
            backgroundColor(String(Theme.Color.lightGrey)),
          ]),
        "timeLineDot":
          style([
            top(Pt(14.)),
            marginRight(Pt(-6.)),
            marginBottom(Pt(-6.)),
            width(Pt(6.)),
            height(Pt(6.)),
            borderWidth(1.),
            borderColor(String(Theme.Color.lightGrey)),
            backgroundColor(String(Theme.Color.background)),
            borderRadius(100.),
          ]),
        "content":
          style([
            flex(1.),
            paddingVertical(Pt(8.)),
            paddingHorizontal(Pt(16.)),
          ]),
        "titleText":
          style([
            paddingBottom(Pt(2.)),
            fontSize(Float(16.)),
            fontFamily("Roboto"),
            fontWeight(`_400),
            color(String(Theme.Color.black)),
          ]),
        "text":
          style([
            paddingBottom(Pt(2.)),
            fontSize(Float(14.)),
            fontFamily("Roboto"),
            fontWeight(`_300),
            color(String(Theme.Color.darkerGrey)),
          ]),
        "sideText":
          style([
            paddingBottom(Pt(2.)),
            fontSize(Float(14.)),
            fontFamily("Roboto"),
            fontWeight(`_300),
            color(String(Theme.Color.darkGrey)),
          ]),
        "tags":
          style([
            flexDirection(Row),
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
          <Text>
            ...(
              scheduleItem##places
              |> Array.to_list
              |> List.map((places) => <Text style=styles##text value=places##name />)
              |> insertSeparates(<Text style=styles##sideText value=" & " />)
              |> Array.of_list
            )
          </Text>
          (
            switch (scheduleItem##places) {
            | a when Array.length(a) > 1 => <Text value="\n" />
            | _ => <Text style=styles##sideText value={j| · |j} />
            }
          )
          <Text
            style=styles##text
            value=Js.String.replace(
              "minute",
              "min",
              DateFns.distanceInWordsStrict(
                getStart((scheduleItem##periods)[0]),
                getEnd((scheduleItem##periods)[Array.length(scheduleItem##periods) - 1]),
              ),
            )
          />
        </Text>
        <View style=styles##tags>
          ...(Array.append(
            switch (scheduleItem##eventInterface) {
            | `Session(session) => [|<Tag value=Tag.Language(session##language) />|]
            | _ => [|ReasonReact.nullElement|]
            },
            switch (scheduleItem##eventInterface) {
            | `Session(session) => Array.map(tag => <Tag value=Tag.Other(tag##name) />, session##tags)
            | _ => [|ReasonReact.nullElement|]
            },
          ))
        </View>
      </TouchableOpacity>
    </View>,
};
