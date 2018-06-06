type state = {scrollViewRef: ref(option(ReasonReact.reactRef))};

type action =
  | NoAction;

let component = ReasonReact.reducerComponent("ScheduleItemDetails");

let setScrollViewRef = (r, {ReasonReact.state}) =>
  state.scrollViewRef := Js.Nullable.toOption(r);

let scrollScrollViewToTop = (_, {ReasonReact.state}) =>
  switch (state.scrollViewRef^) {
  | None => ()
  | Some(r) => ScrollView.scrollTo(r, ~x=0, ~y=0, ~animated=true)
  };

let scrollToTop = r => {
  let self = ReasonReactUtils.refToSelf(r);
  self.handle(scrollScrollViewToTop, ());
};

[@bs.get] external getStart : 'a => Js.Date.t = "start";
[@bs.get] external getEnd : 'a => Js.Date.t = "end";

let rec insertSeparates = (separator: 'a, list: list('a)) => {
  switch (list) {
  | [] => []
  | [head] => [head]
  | [head, ...tail] => [head, separator, ...insertSeparates(separator, tail)]
  }
};

let styles =
  StyleSheet.create(
    Style.(
      {
        "scrollView":
          style([
            flex(1.),
            paddingVertical(Pt(4.)),
            backgroundColor(String(Theme.Color.background)),
          ]),
        "scrollViewContent":
          style([
            paddingVertical(Pt(1.)),
            paddingHorizontal(Pt(24.)),
            paddingBottom(Pt(16.)),
          ]),
        "timeAndPlaceText":
          style([
            paddingVertical(Pt(12.)),
            fontSize(Float(12.9)),
            fontFamily("Roboto Mono"),
            fontWeight(`_300),
            lineHeight(14.),
            letterSpacing(-0.55),
            color(String(Theme.Color.darkerGrey)),
          ]),
        "titleText":
          style([
            paddingBottom(Pt(2.)),
            fontSize(Float(20.)),
            lineHeight(24.),
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
        "sessionTags":
          style([
            flexDirection(Row),
            marginBottom(Pt(16.)),
          ]),
        "sessionDescriptionText":
          style([
            paddingBottom(Pt(2.)),
            fontSize(Float(15.)),
            lineHeight(20.),
            fontFamily("Roboto"),
            fontWeight(`_300),
            color(String(Theme.Color.darkerGrey)),
          ]),
        "wordSpeakerText":
          style([
            marginVertical(Pt(16.)),
            paddingBottom(Pt(2.)),
            fontSize(Float(12.)),
            fontFamily("Roboto Mono"),
            fontWeight(`_400),
            color(String(Theme.Color.black)),
          ]),
        "sessionSpeakerNameText":
          style([
            paddingBottom(Pt(2.)),
            fontSize(Float(15.)),
            fontFamily("Roboto"),
            fontWeight(`_500),
            color(String(Theme.Color.darkerGrey)),
          ]),
        "sessionSpeakerTitleText":
          style([
            paddingBottom(Pt(2.)),
            fontSize(Float(13.)),
            fontFamily("Roboto"),
            fontWeight(`_300),
            color(String(Theme.Color.darkGrey)),
          ]),
      }
    ),
  );

let make = (~scheduleItem, ~onRefresh, ~refreshing, _children) => {
  ...component,
  initialState: () => {scrollViewRef: ref(None)},
  reducer: (_action: action, _state) => ReasonReact.NoUpdate,
  render: self =>
    <ScrollView
      ref=(self.handle(setScrollViewRef))
      style=styles##scrollView
      contentContainerStyle=styles##scrollViewContent
      refreshControl={<RefreshControl refreshing onRefresh />}>
      <Text style=styles##timeAndPlaceText>
        <Text>
          ...(
            scheduleItem##places
            |> Array.to_list
            |> List.map((places) => <Text value=places##name />)
            |> insertSeparates(<Text value=" & " />)
            |> Array.of_list
          )
        </Text>
        (
          switch (scheduleItem##places) {
          /*| a when Array.length(a) > 1 => <Text value="\n" />*/
          | _ => <Text value={j| · |j} />
          }
        )
        <Text
          value=DateFns.distanceInWordsStrict(
            getStart((scheduleItem##periods)[0]),
            getEnd((scheduleItem##periods)[Array.length(scheduleItem##periods) - 1]),
          )
        />
      </Text>
      <Text style=styles##titleText value=scheduleItem##event##title />
      (
        switch (scheduleItem##eventInterface) {
        | `Activity(_activity) =>
          <View>
          </View>
        | `Session(session) =>
          <View>
            <View style=styles##sessionTags>
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
            <Text style=styles##sessionDescriptionText value=session##description />
            <Text style=styles##wordSpeakerText value="SPEAKER" />
            <Text style=styles##sessionSpeakerNameText value=session##speaker##name />
            (
              switch (session##speaker##title) {
              | None => ReasonReact.nullElement
              | Some(title) => <Text style=styles##sessionSpeakerTitleText value=title />
              }
            )
          </View>
        }
      )
    </ScrollView>,
};
