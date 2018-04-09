type state = {
  scrollViewRef: ref(option(ReasonReact.reactRef))
};

type action = NoAction;

let component = ReasonReact.reducerComponent("InformationScreen");

let setScrollViewRef = (r, {ReasonReact.state}) => state.scrollViewRef := Js.Nullable.toOption(r);

let scrollScrollViewToTop = (_, {ReasonReact.state}) =>
  switch state.scrollViewRef^ {
  | None => ()
  | Some(r) => ScrollView.scrollTo(r, ~x=0, ~y=0, ~animated=true)
  };

let scrollToTop = (r) => {
  let self = ReasonReactUtils.refToSelf(r);
  self.handle(scrollScrollViewToTop, ())
};

let styles =
  BsReactNative.(
    StyleSheet.create(
      Style.(
        {
          "container": style([]),
          "content":
            style([
              flex(1.),
              paddingVertical(Pt(128.)),
              paddingHorizontal(Pt(16.)),
              justifyContent(Center),
              alignItems(Center)
            ])
        }
      )
    )
  );

let make = (_children) => {
  ...component,
  initialState: () => {scrollViewRef: ref(None)},
  reducer: (_action: action, _state) => ReasonReact.NoUpdate,
  render: (self) => {
    <ScrollView contentContainerStyle=styles##container ref=(self.handle(setScrollViewRef))>
      <StackNavigator.Header style="default">
        <StackNavigator.Header.TitleText value="Information" />
      </StackNavigator.Header>
      <View style=styles##content>
        <Text value="Information Screen" />
      </View>
    </ScrollView>
  }
};
