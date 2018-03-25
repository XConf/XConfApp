module MainStackNavigator = StackNavigator.Make(MainStackRouting);

type stackNavigation =
  | HomeStack
  | Home2Stack;

type stackNavigationStates = {
  home: MainStackNavigator.navigationState,
  home2: MainStackNavigator.navigationState
};

type state = {
  tabNavigation: TabNavigator.navigationState,
  stackNavigations: stackNavigationStates
};

let updatedStackNavigationState =
    (
      states: stackNavigationStates,
      stackNavigation: stackNavigation,
      state: MainStackNavigator.navigationState
    ) =>
  switch stackNavigation {
  | HomeStack => {...states, home: state}
  | Home2Stack => {...states, home2: state}
  };

let stackNavigationStateFromStackNavigation = (stackNavigation, stackNavigationStates) =>
  switch stackNavigation {
  | HomeStack => stackNavigationStates.home
  | Home2Stack => stackNavigationStates.home2
  };

let stackNavigationFromTabIndex = (index) =>
  switch index {
  | 0 => HomeStack
  | 1 => Home2Stack
  | _ => HomeStack
  };

type action =
  | UpdateTabNavigationState(TabNavigator.navigationState)
  | UpdateStackNavigationState(stackNavigation, MainStackNavigator.navigationState)
  | CurrentStackNavigationJumpToTop;

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
    tabNavigation: TabNavigator.initialState,
    stackNavigations: {
      home: MainStackNavigator.initialStateWithRoute(MainStackRouting.Home),
      home2: MainStackNavigator.initialStateWithRoute(MainStackRouting.Home)
    }
  },
  reducer: (action, state) =>
    switch action {
    | UpdateTabNavigationState(newState) => ReasonReact.Update({...state, tabNavigation: newState})
    | UpdateStackNavigationState(stackNavigation, newState) =>
      ReasonReact.Update({
        ...state,
        stackNavigations:
          updatedStackNavigationState(state.stackNavigations, stackNavigation, newState)
      })
    | CurrentStackNavigationJumpToTop =>
      let currentstackNavigation = stackNavigationFromTabIndex(state.tabNavigation##index);
      ReasonReact.Update({
        ...state,
        stackNavigations:
          updatedStackNavigationState(
            state.stackNavigations,
            currentstackNavigation,
            MainStackNavigator.routePopToToped(
              stackNavigationStateFromStackNavigation(
                currentstackNavigation,
                state.stackNavigations
              )
            )
          )
      })
    },
  render: (self) => {
    let tabBarOnPress = ({TabNavigator.Tab.index}) =>
      if (index === self.state.tabNavigation##index) {
        let state =
          stackNavigationStateFromStackNavigation(
            stackNavigationFromTabIndex(self.state.tabNavigation##index),
            self.state.stackNavigations
          );
        switch state##index {
        | 0 =>
          switch ((state##routes)[0]##route, ((state##routes)[0]##screenRef)^) {
          | (_, None) => ()
          | (MainStackRouting.Home, Some(r)) => HomeScreen.scrollToTop(r)
          | _ => ()
          }
        | _ => self.send(CurrentStackNavigationJumpToTop)
        }
      };
    <TabNavigator
      state=self.state.tabNavigation
      updateState=((newState) => self.send(UpdateTabNavigationState(newState)))>
      <TabNavigator.Tab title="Tab 1" tabBarIcon tabBarOnPress>
        <MainStackNavigator
          state=self.state.stackNavigations.home
          updateState=((newState) => self.send(UpdateStackNavigationState(HomeStack, newState)))
        />
      </TabNavigator.Tab>
      <TabNavigator.Tab title="Tab 2" tabBarIcon tabBarOnPress>
        <MainStackNavigator
          state=self.state.stackNavigations.home2
          updateState=((newState) => self.send(UpdateStackNavigationState(Home2Stack, newState)))
        />
      </TabNavigator.Tab>
    </TabNavigator>
  }
};
