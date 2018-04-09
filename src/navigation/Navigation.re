module AppModalStackNavigator = ModalStackNavigator.Make(ModalStackRouting);
module AppTabNavigator = TabNavigator.Make(MainTabConfig);
module AppStackNavigator = StackNavigator.Make(MainStackRouting);

module StackNavigatorStateMap =
  Map.Make(
    {
      type t = AppTabNavigator.tab;
      let compare = compare;
    }
  );

type stackNavigationsState =
  StackNavigatorStateMap.t(AppStackNavigator.State.t);

type state = {
  modalStackNavigation: AppModalStackNavigator.State.t,
  tabNavigation: AppTabNavigator.State.t,
  stackNavigations: stackNavigationsState
};

type action =
  | UpdateModalStackNavigationState(AppModalStackNavigator.State.updator)
  | UpdateTabNavigationState(AppTabNavigator.State.updator)
  | UpdateStackNavigationState(
      MainTabConfig.tab,
      AppStackNavigator.State.updator
    )
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
      {AppTabNavigator.activedTab},
      {
        ReasonReact.send,
        ReasonReact.state: {stackNavigations, tabNavigation: {activeTab}}
      }
    ) =>
  switch (
    activedTab,
    StackNavigatorStateMap.find(activedTab, stackNavigations)
  ) {
  | (
      activedTab,
      {
        index: 0,
        routes: [
          {route: MainStackRouting.Schedule, screenRef: {contents: Some(r)}},
          ..._
        ]
      }
    )
      when activedTab == activeTab =>
    ScheduleScreen.scrollToTop(r)
  | (
      activedTab,
      {
        index: 0,
        routes: [
          {route: MainStackRouting.Information, screenRef: {contents: Some(r)}},
          ..._
        ]
      }
    )
      when activedTab == activeTab =>
    InformationScreen.scrollToTop(r)
  | (activedTab, {index: i}) when activedTab == activeTab && i > 0 =>
    send(PopCurrentStackNavigationToTop)
  | _ => ()
  };

let appStackNavigatorEvent =
    (
      event: MainStackRouting.screenEvent,
      {ReasonReact.send}
    ) =>
  switch event {
  | MainStackRouting.OpenMap =>
    send(
      UpdateModalStackNavigationState(
        AppModalStackNavigator.State.routePushed(ModalStackRouting.Map)
      )
    )
  };

let make = (_children) => {
  ...component,
  initialState: () => {
    modalStackNavigation: AppModalStackNavigator.initialState,
    tabNavigation:
      AppTabNavigator.initialStateWithDefaultTab(MainTabConfig.Schedule),
    stackNavigations:
      StackNavigatorStateMap.empty
      |> StackNavigatorStateMap.add(
           MainTabConfig.Schedule,
           AppStackNavigator.initialStateWithRoute(MainStackRouting.Schedule)
         )
      |> StackNavigatorStateMap.add(
           MainTabConfig.Information,
           AppStackNavigator.initialStateWithRoute(MainStackRouting.Information)
         )
  },
  reducer: (action, state) =>
    switch action {
    | UpdateModalStackNavigationState(updator) =>
      ReasonReact.Update({
        ...state,
        modalStackNavigation: updator(state.modalStackNavigation)
      })
    | UpdateTabNavigationState(updator) =>
      ReasonReact.Update({
        ...state,
        tabNavigation: updator(state.tabNavigation)
      })
    | UpdateStackNavigationState(tab, updator) =>
      ReasonReact.Update({
        ...state,
        stackNavigations:
          StackNavigatorStateMap.add(
            tab,
            updator(StackNavigatorStateMap.find(tab, state.stackNavigations)),
            state.stackNavigations
          )
      })
    | PopCurrentStackNavigationToTop =>
      let currentTab = state.tabNavigation.activeTab;
      ReasonReact.Update({
        ...state,
        stackNavigations:
          StackNavigatorStateMap.add(
            currentTab,
            AppStackNavigator.State.routePopToToped(
              StackNavigatorStateMap.find(currentTab, state.stackNavigations)
            ),
            state.stackNavigations
          )
      })
    },
  render: (self) =>
    <AppModalStackNavigator
      state=self.state.modalStackNavigation
      updateState=(
        (updator) => self.send(UpdateModalStackNavigationState(updator))
      )>
      <AppTabNavigator
        state=self.state.tabNavigation
        updateState=((updator) => self.send(UpdateTabNavigationState(updator)))>
        <AppTabNavigator.Tab
          title="Schedule" tabBarIcon tabBarOnPress=(self.handle(tabBarPress))>
          <AppStackNavigator
            state=(
              StackNavigatorStateMap.find(
                MainTabConfig.Schedule,
                self.state.stackNavigations
              )
            )
            updateState=(
              (updator) =>
                self.send(
                  UpdateStackNavigationState(MainTabConfig.Schedule, updator)
                )
            )
            handleEvent=(self.handle(appStackNavigatorEvent))
          />
        </AppTabNavigator.Tab>
        <AppTabNavigator.Tab
          title="Information" tabBarIcon tabBarOnPress=(self.handle(tabBarPress))>
          <AppStackNavigator
            state=(
              StackNavigatorStateMap.find(
                MainTabConfig.Information,
                self.state.stackNavigations
              )
            )
            updateState=(
              (updator) =>
                self.send(
                  UpdateStackNavigationState(MainTabConfig.Information, updator)
                )
            )
          />
        </AppTabNavigator.Tab>
      </AppTabNavigator>
    </AppModalStackNavigator>
};
