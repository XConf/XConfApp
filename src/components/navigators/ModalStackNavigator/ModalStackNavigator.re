[@bs.module "./ModalStackNavigatorJS"] external jsModalStackNavigator : ReasonReact.reactClass =
  "default";

type routerUtils('route) = {
  pushRoute: 'route => unit,
  popRoute: unit => unit
};

module type Routing = {
  type route;
  let router: (route, ~utils: routerUtils(route)) => ReasonReact.reactElement;
};

module Make = (R: Routing) => {
  type routeEntry = {
    route: R.route,
    key: string /*,
    screenRef: ref(option(ReasonReact.reactRef))*/
  };
  type navigationState = {
    index: int,
    routes: list(routeEntry)
  };
  let random = () : string => string_of_int(Random.bits()) ++ "-" ++ string_of_int(Random.bits());
  let initialState: navigationState = {index: (-1), routes: []};
  module State = {
    let routePushed = (route: R.route, state: navigationState) : navigationState => {
      index: state.index + 1,
      routes: [{key: random(), route /*, screenRef: ref(None)*/}, ...state.routes]
    };
    let routePoped = (state: navigationState) : navigationState =>
      switch (state.index, state.routes) {
      | ((-1), _) => state
      | (index, [_, ...leftoveredRoutes]) => {index: index - 1, routes: leftoveredRoutes}
      | _ => state
      };
    let rec onlyLast =
      fun
      | [] => []
      | [a] => [a]
      | [_, ...t] => onlyLast(t);
    let routePopToToped = (state: navigationState) : navigationState =>
      switch state.index {
      | (-1) => state
      | _ => {index: (-1), routes: onlyLast(state.routes)}
      };
  };
  let make = (~state: navigationState, ~updateState: navigationState => unit, children) => {
    let routerUtils = {
      pushRoute: (route: R.route) => State.routePushed(route, state) |> updateState,
      popRoute: () => State.routePoped(state) |> updateState
    };
    ReasonReact.wrapJsForReason(
      ~reactClass=jsModalStackNavigator,
      ~props={
        "router": R.router(~utils=routerUtils),
        "state": state,
        "handleBack": routerUtils.popRoute
      },
      children
    )
  };
};
