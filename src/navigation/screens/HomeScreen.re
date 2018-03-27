type state = {
  count: int,
  increment: int,
  scrollViewRef: ref(option(ReasonReact.reactRef))
};

type action =
  | Plus
  | SetIncrement(int);

let component = ReasonReact.reducerComponent("HomeScreen");

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
          "mainContent":
            style([
              flex(1.),
              paddingVertical(Pt(128.)),
              paddingHorizontal(Pt(16.)),
              justifyContent(Center),
              alignItems(Center)
            ]),
          "loremIpsum": style([paddingHorizontal(Pt(16.))]),
          "bottomActions":
            style([
              flex(1.),
              paddingVertical(Pt(32.)),
              paddingHorizontal(Pt(16.)),
              justifyContent(Center),
              alignItems(Center)
            ])
        }
      )
    )
  );

let make = (~onInfoPress: unit => unit, ~onGoToDetailsPress: unit => unit, _children) => {
  ...component,
  initialState: () => {count: 0, increment: 1, scrollViewRef: ref(None)},
  reducer: (action, state) =>
    switch action {
    | Plus => ReasonReact.Update({...state, count: state.count + state.increment})
    | SetIncrement(increment) => ReasonReact.Update({...state, increment})
    },
  render: (self) => {
    let message = "Count: " ++ string_of_int(self.state.count);
    let increment = string_of_int(self.state.increment);
    <ScrollView contentContainerStyle=styles##container ref=(self.handle(setScrollViewRef))>
      <StackNavigator.Header style="default">
        <StackNavigator.Header.Left>
          <Button title="Info" onPress=onInfoPress />
        </StackNavigator.Header.Left>
        <StackNavigator.Header.TitleText value="Home" />
        <StackNavigator.Header.Right>
          <Button title={j|+$increment|j} onPress=((_event) => self.send(Plus)) />
        </StackNavigator.Header.Right>
      </StackNavigator.Header>
      <View style=styles##mainContent>
        <Text value="Home Screen" />
        <Text />
        <Text value=message />
        <Text />
        <Button title="Set Plus Increment To 1" onPress=((_event) => self.send(SetIncrement(1))) />
        <Button
          title="Set Plus Increment To 10"
          onPress=((_event) => self.send(SetIncrement(10)))
        />
        <Text />
        <Button title="Go To Details Screen" onPress=onGoToDetailsPress />
      </View>
      <View style=styles##loremIpsum>
        <Text
          value="Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
        />
        <Text
          value="Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
        />
        <Text
          value="Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
        />
        <Text
          value="Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
        />
        <Text
          value="Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
        />
        <Text
          value="Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
        />
      </View>
      <View style=styles##bottomActions>
        <Button title="Go To Details Screen" onPress=onGoToDetailsPress />
        <Button title="Scroll to Top" onPress=(self.handle(scrollScrollViewToTop)) />
      </View>
    </ScrollView>
  }
};
