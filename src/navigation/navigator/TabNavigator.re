[@bs.module "./TabNavigatorJS"] external jsTabNavigator : ReasonReact.reactClass = "default";

type navigationState = {. "index": int};

let initialState: navigationState = {"index": 0};

let make = (~state: navigationState, ~updateState: navigationState => unit, children) => {
  ReasonReact.wrapJsForReason(
    ~reactClass=jsTabNavigator,
    ~props={
      "state": state,
      "updateState": updateState
    },
    children
  )
};
