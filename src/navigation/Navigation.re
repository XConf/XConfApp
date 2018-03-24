module MainStackNavigator = StackNavigator.Make(MainStackRouting);

type state = {
  tabNavigationState: TabNavigator.navigationState,
  stackNavigation1State: MainStackNavigator.navigationState,
  stackNavigation2State: MainStackNavigator.navigationState
};

type action =
  | UpdateTabNavigationState(TabNavigator.navigationState)
  | UpdateStackNavigation1State(MainStackNavigator.navigationState)
  | UpdateStackNavigation2State(MainStackNavigator.navigationState);

let component = ReasonReact.reducerComponent("Navigation");

let tabBarIcon = (~focused, ~tintColor: string) => {
  let tc: string = tintColor;
  <Text
    style=BsReactNative.Style.(style([color(tc)]))
    value=(focused ? {j|'ω'|j} : {j|°ω°|j})
  />
};

let make = (_children) => {
  ...component,
  initialState: () => {
    tabNavigationState: TabNavigator.initialState,
    stackNavigation1State: MainStackNavigator.initialStateForRoute(MainStackRouting.Home),
    stackNavigation2State: MainStackNavigator.initialStateForRoute(MainStackRouting.Home)
  },
  reducer: (action, state) =>
    switch action {
    | UpdateTabNavigationState(newState) =>
      ReasonReact.Update({...state, tabNavigationState: newState})
    | UpdateStackNavigation1State(newState) =>
      ReasonReact.Update({...state, stackNavigation1State: newState})
    | UpdateStackNavigation2State(newState) =>
      ReasonReact.Update({...state, stackNavigation2State: newState})
    },
  render: (self) =>
    <TabNavigator
      state=self.state.tabNavigationState
      updateState=((newState) => self.send(UpdateTabNavigationState(newState)))>
      <TabNavigator.Tab title="Tab 1" tabBarIcon>
        <MainStackNavigator
          state=self.state.stackNavigation1State
          updateState=((newState) => self.send(UpdateStackNavigation1State(newState)))
        />
      </TabNavigator.Tab>
      <TabNavigator.Tab title="Tab 2" tabBarIcon>
        <MainStackNavigator
          state=self.state.stackNavigation2State
          updateState=((newState) => self.send(UpdateStackNavigation2State(newState)))
        />
      </TabNavigator.Tab>
    </TabNavigator>
};
