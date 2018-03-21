[@bs.module "./StackNavigatorJS"] external jsStackNavigator : ReasonReact.reactClass = "default";
[@bs.module "./StackNavigatorJS"] external jsStackNavigatorHeader : ReasonReact.reactClass = "Header";
[@bs.module "./StackNavigatorJS"] external jsStackNavigatorHeaderTitleText : ReasonReact.reactClass = "HeaderTitleText";
[@bs.module "./StackNavigatorJS"] external jsStackNavigatorHeaderRight : ReasonReact.reactClass = "HeaderRight";
[@bs.module "./StackNavigatorJS"] external jsStackNavigatorHeaderLeft : ReasonReact.reactClass = "HeaderLeft";

module type Routing = {
  type route;
  let router:
    (route, ~pushRoute: route => unit, ~popRoute: unit => unit) => ReasonReact.reactElement;
};

module Make = (R: Routing) => {
  type routeEntry = {. "route": R.route, "key": string};
  type navigationState = {. "index": int, "routes": array(routeEntry)};

  let random = () : string => string_of_int(Random.bits()) ++ "-" ++ string_of_int(Random.bits());

  let initialStateForRoute = (route: R.route) : navigationState => {
    "index": 0,
    "routes": [|{"key": random(), "route": route}|]
  };
  let pushRouteToState = (route: R.route, state: navigationState) : navigationState => {
    "index": state##index + 1,
    "routes": Array.append(state##routes, [|{"key": random(), "route": route}|])
  };
  let popRouteFromState = (state: navigationState) : navigationState =>
    switch state##index {
    | 0 => state
    | _ => {
        "index": state##index - 1,
        "routes": Array.sub(state##routes, 0, Array.length(state##routes) - 1)
      }
    };

  let make = (~state: navigationState, ~updateState: navigationState => unit, children) => {
    let pushRoute = (route: R.route) =>
      pushRouteToState(route, state)
        |> updateState;
    let popRoute = () =>
      popRouteFromState(state)
        |> updateState;

    ReasonReact.wrapJsForReason(
      ~reactClass=jsStackNavigator,
      ~props={
        "router": R.router(~pushRoute, ~popRoute),
        "state": state,
        "updateState": updateState
      },
      children
    )
  };
};

module Header {
  let make = (~style: string, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=jsStackNavigatorHeader,
      ~props={"style": style},
      children
    );

  module TitleText {
    let make = (~value: string, children) =>
      ReasonReact.wrapJsForReason(
        ~reactClass=jsStackNavigatorHeaderTitleText,
        ~props={"value": value},
        children
      );
  };

  module Right {
    let make = (children) =>
      ReasonReact.wrapJsForReason(
        ~reactClass=jsStackNavigatorHeaderRight,
        ~props=Js.Obj.empty(),
        children
      );
  };

  module Left {
    let make = (children) =>
      ReasonReact.wrapJsForReason(
        ~reactClass=jsStackNavigatorHeaderLeft,
        ~props=Js.Obj.empty(),
        children
      );
  };
}
