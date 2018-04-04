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
  let random = () : string => string_of_int(Random.bits()) ++ "-" ++ string_of_int(Random.bits());
  module State = {
    type routeEntry = {
      route: R.route,
      key: string /*,
      screenRef: ref(option(ReasonReact.reactRef))*/
    };
    type t = {
      index: int,
      routes: list(routeEntry)
    };
    type updator = t => t;
    let routePushed = (route: R.route, state: t) : t => {
      index: state.index + 1,
      routes: [{key: random(), route /*, screenRef: ref(None)*/}, ...state.routes]
    };
    let routePoped = (state: t) : t =>
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
    let routePopToToped = (state: t) : t =>
      switch state.index {
      | (-1) => state
      | _ => {index: (-1), routes: onlyLast(state.routes)}
      };
  };
  let initialState: State.t = {index: (-1), routes: []};
  let getRouterUtilsFromUpdateState = updateState => {
    pushRoute: (route: R.route) => State.routePushed(route) |> updateState,
    popRoute: () => State.routePoped |> updateState
  };
  let getUtilsAppliedRouter = utils => R.router(~utils=utils);
  let make = (~state: State.t, ~updateState: State.updator => unit, children) => {
    ReasonReact.wrapJsForReason(
      ~reactClass=jsModalStackNavigator,
      ~props={
        "router": R.router,
        "state": state,
        "updateState": updateState,
        "getRouterUtilsFromUpdateState": getRouterUtilsFromUpdateState,
        "getUtilsAppliedRouter": getUtilsAppliedRouter
      },
      children
    )
  };
};
