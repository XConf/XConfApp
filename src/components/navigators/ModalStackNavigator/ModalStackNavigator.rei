type routerUtils('route) = {
  pushRoute: 'route => unit,
  popRoute: unit => unit
};

module type Routing = {
  type route;
  let router: (route, ~utils: routerUtils(route)) => ReasonReact.reactElement;
};

module Make:
  (R: Routing) =>
  {
    type routeEntry = {
      route: R.route,
      key: string /*,
      screenRef: ref(option(ReasonReact.reactRef))*/
    };
    type navigationState = {
      index: int,
      routes: list(routeEntry)
    };
    let initialState: navigationState;
    module State: {
      let routePushed: (R.route, navigationState) => navigationState;
      let routePoped: navigationState => navigationState;
      let routePopToToped: navigationState => navigationState;
    };
    let make:
      (~state: navigationState, ~updateState: navigationState => unit, array('a)) =>
      ReasonReact.component(
        ReasonReact.stateless,
        ReasonReact.noRetainedProps,
        ReasonReact.actionless
      );
  };
