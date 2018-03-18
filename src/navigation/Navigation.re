module MainStackNavigator = StackNavigator.Make(MainStackRouting);

type state = {
  navigation: MainStackNavigator.navigationState
};

type action =
  | Update(MainStackNavigator.navigationState);

let component = ReasonReact.reducerComponent("Navigation");

let make = (_children) => {
  ...component,

  initialState: () => {
    navigation: MainStackNavigator.initialStateForRoute(MainStackRouting.Home)
  },

  reducer: (action, state) =>
    switch (action) {
    | Update(newState) => ReasonReact.Update({...state, navigation: newState})
    },

  render: self => {
    <MainStackNavigator
      state=self.state.navigation
      updateState=(newState => self.send(Update(newState)))
    />;
  }
};
