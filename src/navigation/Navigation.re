module MainStackNavigator = StackNavigator.Make(MainStackRouting);

module MainTabNavigator = TabNavigator.Make(MainTab);

module StackNavigatorStateMap =
  Map.Make(
    {
      type t = MainTabNavigator.tab;
      let compare = compare;
    }
  );

type stackNavigationsState = StackNavigatorStateMap.t(MainStackNavigator.navigationState);

type state = {
  tabNavigation: MainTabNavigator.navigationState,
  stackNavigations: stackNavigationsState
};

type action =
  | UpdateTabNavigationState(MainTabNavigator.navigationState)
  | UpdateStackNavigationState(MainTab.tab, MainStackNavigator.navigationState)
  | PopCurrentStackNavigationToTop;

let component = ReasonReact.reducerComponent("Navigation");

let tabBarIcon = (~focused, ~tintColor: string) => {
  let tc: string = tintColor;
  <Text
    style=BsReactNative.Style.(style([color(tc)]))
    value=(focused ? {j|'ω'|j} : {j|°ω°|j})
  />
};

let tabBarPress =
    (
      {MainTabNavigator.activedTab},
      {ReasonReact.send, ReasonReact.state: {stackNavigations, tabNavigation: {activeTab}}}
    ) =>
  switch (activedTab, StackNavigatorStateMap.find(activedTab, stackNavigations)) {
  | (
      activedTab,
      {index: 0, routes: [{route: MainStackRouting.Home, screenRef: {contents: Some(r)}}, ..._]}
    )
      when activedTab == activeTab =>
    HomeScreen.scrollToTop(r)
  | (activedTab, {index: i}) when activedTab == activeTab && i > 0 =>
    send(PopCurrentStackNavigationToTop)
  | _ => ()
  };

let make = (_children) => {
  ...component,
  initialState: () => {
    tabNavigation: MainTabNavigator.initialStateWithDefaultTab(MainTab.Home),
    stackNavigations:
      StackNavigatorStateMap.empty
      |> StackNavigatorStateMap.add(
           MainTab.Home,
           MainStackNavigator.initialStateWithRoute(MainStackRouting.Home)
         )
      |> StackNavigatorStateMap.add(
           MainTab.Home2,
           MainStackNavigator.initialStateWithRoute(MainStackRouting.Home)
         )
  },
  reducer: (action, state) =>
    switch action {
    | UpdateTabNavigationState(newState) => ReasonReact.Update({...state, tabNavigation: newState})
    | UpdateStackNavigationState(tab, newState) =>
      ReasonReact.Update({
        ...state,
        stackNavigations: StackNavigatorStateMap.add(tab, newState, state.stackNavigations)
      })
    | PopCurrentStackNavigationToTop =>
      let currentTab = state.tabNavigation.activeTab;
      ReasonReact.Update({
        ...state,
        stackNavigations:
          StackNavigatorStateMap.add(
            currentTab,
            MainStackNavigator.routePopToToped(
              StackNavigatorStateMap.find(currentTab, state.stackNavigations)
            ),
            state.stackNavigations
          )
      })
    },
  render: (self) =>
    <MainTabNavigator
      state=self.state.tabNavigation
      updateState=((newState) => self.send(UpdateTabNavigationState(newState)))>
      <MainTabNavigator.Tab title="Tab 1" tabBarIcon tabBarOnPress=(self.handle(tabBarPress))>
        <MainStackNavigator
          state=(StackNavigatorStateMap.find(MainTab.Home, self.state.stackNavigations))
          updateState=(
            (newState) => self.send(UpdateStackNavigationState(MainTab.Home, newState))
          )
        />
      </MainTabNavigator.Tab>
      <MainTabNavigator.Tab title="Tab 2" tabBarIcon tabBarOnPress=(self.handle(tabBarPress))>
        <MainStackNavigator
          state=(StackNavigatorStateMap.find(MainTab.Home2, self.state.stackNavigations))
          updateState=(
            (newState) => self.send(UpdateStackNavigationState(MainTab.Home2, newState))
          )
        />
      </MainTabNavigator.Tab>
    </MainTabNavigator>
};
