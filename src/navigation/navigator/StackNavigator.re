[@bs.module "./StackNavigatorJS"] external jsStackNavigator : ReasonReact.reactClass = "default";

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
  let popRouteFromState = (state: navigationState) : navigationState => {
    "index": state##index - 1,
    "routes": Array.sub(state##routes, 0, Array.length(state##routes) - 1)
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
/*
 type routeEntry = {. "route": Route.mainStackRoute, "key": string};

 type navigationState = {. "index": int, "routes": array(routeEntry)};

 let make = (~router: (Route.mainStackRoute) => ReasonReact.reactElement, ~state: navigationState, ~updateState: navigationState => unit, children) =>
   ReasonReact.wrapJsForReason(
     ~reactClass=jsStackNavigator,
     ~props={"router": router, "state": state, "updateState": updateState},
     children
   );

 let random = () : string => string_of_int(Random.bits()) ++ "-" ++ string_of_int(Random.bits());

 let initialStateForRoute = (route: Route.mainStackRoute) : navigationState => {
   "index": 0,
   "routes": [|{"key": random(), "route": route}|]
 };

 let pushRouteToState = (route: Route.mainStackRoute, state: navigationState) : navigationState => {
   "index": state##index + 1,
   "routes": Array.append(state##routes, [|{"key": random(), "route": route}|])
 };

 let popRouteFromState = (state: navigationState) : navigationState => {
   "index": state##index - 1,
   "routes": Array.sub(state##routes, 0, Array.length(state##routes) - 1)
 };
 */
