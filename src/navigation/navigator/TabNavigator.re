[@bs.module "./TabNavigatorJS"] external jsTabNavigator : ReasonReact.reactClass = "default";

[@bs.module "./TabNavigatorJS"] external jsTabNavigatorTab : ReasonReact.reactClass = "Tab";

type navigationState = {. "index": int};

let initialState: navigationState = {"index": 0};

let make = (~state: navigationState, ~updateState: navigationState => unit, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=jsTabNavigator,
    ~props={"state": state, "updateState": updateState},
    children
  );

module Tab = {
  type tabBarOnPressPayload = {
    index: int,
    previousIndex: option(int)
  };
  let toTabBarOnPressPayload = (obj) => {index: obj##index, previousIndex: None};
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
