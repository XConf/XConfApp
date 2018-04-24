type state = {scrollViewRef: ref(option(ReasonReact.reactRef))};

type action =
  | NoAction;

let component = ReasonReact.reducerComponent("ConferenceInformation");

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

let maxDate = fun
  | [] => invalid_arg("empty list")
  | [hd, ...tl] => List.fold_left((a, b) => DateFns.compareAsc(a, b) > 0 ? a : b, hd, tl)
;

let minDate = fun
  | [] => invalid_arg("empty list")
  | [hd, ...tl] => List.fold_left((a, b) => DateFns.compareAsc(a, b) < 0 ? a : b, hd, tl)
;

let rangeOfDates = l => (minDate(l), maxDate(l));

let dateRangeInWords = DateFns.(
  fun
    | (st, ed) when getMonth(st) === getMonth(ed) && getYear(st) === getYear(ed) =>
      DateFns.format("MMM D ", st) ++ "-" ++ DateFns.format(" D, YYYY", ed)
    | (st, ed) when getYear(st) === getYear(ed) =>
      DateFns.format("MMM D ", st) ++ "-" ++ DateFns.format(" MMM D, YYYY", ed)
    | (st, ed) =>
      DateFns.format("MMM D, YYYY ", st) ++ "-" ++ DateFns.format(" MMM D, YYYY", ed)
);

let styles =
  StyleSheet.create(
    Style.(
      {
        "scrollView": style([
          flex(1.),
          backgroundColor(Theme.Color.background),
        ]),
        "scrollViewContent": style([
          /*paddingBottom(Pt(16.)),*/
        ]),
        "keyVisionImage": style([
          height(Pt(230.)),
        ]),
        "content": style([
          paddingVertical(Pt(16.)),
          paddingHorizontal(Pt(24.)),
        ]),
        "conferenceNameText":
          style([
            paddingBottom(Pt(2.)),
            fontSize(Float(20.)),
            lineHeight(24.),
            fontFamily("Roboto"),
            fontWeight(`_400),
            color(Theme.Color.black),
          ]),
        "conferenceDateText":
          style([
            paddingBottom(Pt(2.)),
            fontSize(Float(14.)),
            fontFamily("Roboto"),
            fontWeight(`_300),
            color(Theme.Color.darkGrey),
          ]),
        "conferenceLocationNameText":
          style([
            paddingBottom(Pt(2.)),
            fontSize(Float(14.)),
            fontFamily("Roboto"),
            fontWeight(`_400),
            color(Theme.Color.darkGrey),
          ]),
        "conferenceLocationAddressText":
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

let make = (~conference, ~onRefresh, ~refreshing, _children) => {
  ...component,
  initialState: () => {scrollViewRef: ref(None)},
  reducer: (_action: action, _state) => ReasonReact.NoUpdate,
  render: self => {
    let dateRange =
      conference##dates
      |> Array.map(d => d##date)
      |> Array.to_list
      |> rangeOfDates;
    <ScrollView
      ref=(self.handle(setScrollViewRef))
      style=styles##scrollView
      contentContainerStyle=styles##scrollViewContent
      refreshControl={<RefreshControl refreshing onRefresh />}>
      <Image
        source=URI(Image.imageURISource(~uri=conference##keyVisionUrl, ()))
        style=styles##keyVisionImage
        resizeMode=`cover
      />
      <View style=styles##content>
        <Text style=styles##conferenceNameText value=conference##name />
        <Text style=styles##conferenceDateText value=dateRangeInWords(dateRange) />
        <Text style=styles##conferenceLocationNameText value=conference##location##name />
        <Text style=styles##conferenceLocationAddressText value=conference##location##address />
      </View>
    </ScrollView>
  },
};
