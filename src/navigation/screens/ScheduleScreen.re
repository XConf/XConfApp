type state = {
  scrollViewRef: ref(option(ReasonReact.reactRef))
};

type action = NoAction;

let component = ReasonReact.reducerComponent("ScheduleScreen");

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

let make = (~onMapPress: unit => unit, _children) => {
  ...component,
  initialState: () => {scrollViewRef: ref(None)},
  reducer: (_action: action, _state) => ReasonReact.NoUpdate,
  render: (self) => {
    <ScrollView contentContainerStyle=styles##container ref=(self.handle(setScrollViewRef))>
      <StackNavigator.Header style="default">
        <StackNavigator.Header.Left>
          <Button title="Map" onPress=onMapPress />
        </StackNavigator.Header.Left>
        <StackNavigator.Header.TitleText value="Schedule" />
      </StackNavigator.Header>
      <View style=styles##content>
        <Text value="Schedule Screen" />
      </View>
    </ScrollView>
  }
};
