module AppModalStackNavigator = ModalStackNavigator.Make(ModalStackRouting);
module MainTabNavigator = TabNavigator.Make(MainTabConfig);
module MainStackNavigator = StackNavigator.Make(MainStackRouting);

module StackNavigatorStateMap =
  Map.Make(
    {
      type t = MainTabNavigator.tab;
      let compare = compare;
    }
  );

type stackNavigationsState = StackNavigatorStateMap.t(MainStackNavigator.State.t);

type state = {
  modalStackNavigation: AppModalStackNavigator.State.t,
  tabNavigation: MainTabNavigator.State.t,
  stackNavigations: stackNavigationsState
};

type action =
  | UpdateModalStackNavigationState(AppModalStackNavigator.State.updator)
  | UpdateTabNavigationState(MainTabNavigator.State.updator)
  | UpdateStackNavigationState(MainTabConfig.tab, MainStackNavigator.State.updator)
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

let mainStackNavigatorEvent =
    (event: MainStackRouting.screenEvent, {ReasonReact.state, ReasonReact.send}) =>
  switch event {
  | MainStackRouting.OpenInfo =>
    send(
      UpdateModalStackNavigationState(
        AppModalStackNavigator.State.routePushed(
          ModalStackRouting.Info        )
      )
    )
  };

let make = (_children) => {
  ...component,
  initialState: () => {
    modalStackNavigation: AppModalStackNavigator.initialState,
    tabNavigation: MainTabNavigator.initialStateWithDefaultTab(MainTabConfig.Home),
    stackNavigations:
      StackNavigatorStateMap.empty
      |> StackNavigatorStateMap.add(
           MainTabConfig.Home,
           MainStackNavigator.initialStateWithRoute(MainStackRouting.Home)
         )
      |> StackNavigatorStateMap.add(
           MainTabConfig.Home2,
           MainStackNavigator.initialStateWithRoute(MainStackRouting.Home)
         )
  },
  reducer: (action, state) =>
    switch action {
    | UpdateModalStackNavigationState(updator) =>
      ReasonReact.Update({...state, modalStackNavigation: updator(state.modalStackNavigation)})
    | UpdateTabNavigationState(updator) => ReasonReact.Update({...state, tabNavigation: updator(state.tabNavigation)})
    | UpdateStackNavigationState(tab, updator) =>
      ReasonReact.Update({
        ...state,
        stackNavigations: StackNavigatorStateMap.add(tab, updator(StackNavigatorStateMap.find(tab, state.stackNavigations)), state.stackNavigations)
      })
    | PopCurrentStackNavigationToTop =>
      let currentTab = state.tabNavigation.activeTab;
      ReasonReact.Update({
        ...state,
        stackNavigations:
          StackNavigatorStateMap.add(
            currentTab,
            MainStackNavigator.State.routePopToToped(
              StackNavigatorStateMap.find(currentTab, state.stackNavigations)
            ),
            state.stackNavigations
          )
      })
    },
  render: (self) =>
    <AppModalStackNavigator
      state=self.state.modalStackNavigation
      updateState=((updator) => self.send(UpdateModalStackNavigationState(updator)))>
      <MainTabNavigator
        state=self.state.tabNavigation
        updateState=((updator) => self.send(UpdateTabNavigationState(updator)))>
        <MainTabNavigator.Tab title="Tab 1" tabBarIcon tabBarOnPress=(self.handle(tabBarPress))>
          <MainStackNavigator
            state=(StackNavigatorStateMap.find(MainTabConfig.Home, self.state.stackNavigations))
            updateState=(
              (updator) => self.send(UpdateStackNavigationState(MainTabConfig.Home, updator))
            )
            handleEvent=(self.handle(mainStackNavigatorEvent))
          />
        </MainTabNavigator.Tab>
        <MainTabNavigator.Tab title="Tab 2" tabBarIcon tabBarOnPress=(self.handle(tabBarPress))>
          <MainStackNavigator
            state=(StackNavigatorStateMap.find(MainTabConfig.Home2, self.state.stackNavigations))
            updateState=(
              (updator) => self.send(UpdateStackNavigationState(MainTabConfig.Home2, updator))
            )
          />
        </MainTabNavigator.Tab>
      </MainTabNavigator>
    </AppModalStackNavigator>
};
