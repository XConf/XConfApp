type state = {
  count: int,
  increment: int
};

type action =
  | Plus
  | SetIncrement(int);

let component = ReasonReact.reducerComponent("HomeScreen");

let styles =
  BsReactNative.(
    StyleSheet.create(
      Style.(
        {
          "container": style([
            flex(1.),
            justifyContent(Center),
            alignItems(Center)
          ])
        }
      )
    )
  );

let make = (~onGoToDetailsPress: unit => unit, _children) => {
  ...component,
  initialState: () => {count: 0, increment: 1},
  reducer: (action, state) =>
    switch action {
    | Plus => ReasonReact.Update({...state, count: state.count + state.increment})
    | SetIncrement(increment) => ReasonReact.Update({...state, increment})
    },
  render: (self) => {
    let message = "Count: " ++ string_of_int(self.state.count);
    let increment = string_of_int(self.state.increment);
    <View style=styles##container>
      <StackNavigatorHeader style="default">
        <StackNavigatorHeader.TitleText value="Home" />
        <StackNavigatorHeader.HeaderRight>
          <Button title={j|+$increment|j} onPress=((_event) => self.send(Plus)) />
        </StackNavigatorHeader.HeaderRight>
      </StackNavigatorHeader>
      <Text value="Home Screen" />
      <Text />
      <Text value=message />
      <Text />
      <Button title="Set Plus Increment To 1" onPress=((_event) => self.send(SetIncrement(1))) />
      <Button title="Set Plus Increment To 10" onPress=((_event) => self.send(SetIncrement(10))) />
      <Text />
      <Button title="Go To Details Screen" onPress=onGoToDetailsPress />
    </View>
  }
};
