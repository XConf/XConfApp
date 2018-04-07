[@bs.module "./TabNavigatorJS"] external jsTabNavigator : ReasonReact.reactClass = "default";

[@bs.module "./TabNavigatorJS"] external jsTabNavigatorTab : ReasonReact.reactClass = "Tab";

module type Config = {type tab; let numberOfTabs: int;};

type jsState = {. "index": int};

module Make = (C: Config) => {
  type tab = C.tab;
  module State = {
    type t = {activeTab: tab};
    type updator = t => t;
    let tabFromInt: int => tab = [%bs.raw {| a => a |}];
    let intFromTab: tab => int = [%bs.raw {| a => a |}];
    let jsStateFromState: t => jsState = (state) => {"index": intFromTab(state.activeTab)};
    let stateFromJsState: jsState => t = (jsState) => {activeTab: tabFromInt(jsState##index)};
  };
  let initialStateWithDefaultTab: tab => State.t = (tab) => {activeTab: tab};
  type tabBarOnPressPayload = {
    activedTab: tab,
    previousActiveTab: option(tab)
  };
  let make = (~state: State.t, ~updateState: State.updator => unit, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=jsTabNavigator,
      ~props={
        "state": State.jsStateFromState(state),
        "updateState": (jsState) => updateState((_) => State.stateFromJsState(jsState)),
        "numberOfTabs": C.numberOfTabs
      },
      children
    );
  module Tab = {
    let toTabBarOnPressPayload = (obj) => {activedTab: obj##index, previousActiveTab: None};
    let make =
        (
          ~title: string,
          ~tabBarIcon: (~focused: bool, ~tintColor: string) => ReasonReact.reactElement,
          ~tabBarOnPress: option((tabBarOnPressPayload => unit))=?,
          children
        ) =>
      ReasonReact.wrapJsForReason(
        ~reactClass=jsTabNavigatorTab,
        ~props=
          Js.Undefined.(
            {
              "title": title,
              "tabBarIcon": (obj) =>
                tabBarIcon(~focused=Js.to_bool(obj##focused), ~tintColor=obj##tintColor),
              "tabBarOnPress":
                fromOption(
                  switch tabBarOnPress {
                  | None => None
                  | Some(f) => Some(((obj) => f(toTabBarOnPressPayload(obj))))
                  }
                )
            }
          ),
        children
      );
  };
};
