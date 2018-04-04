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
    module State: {
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
      let routePushed: (R.route, t) => t;
      let routePoped: t => t;
      let routePopToToped: t => t;
    };
    let initialState: State.t;
    let make:
      (
        ~state: State.t,
        ~updateState: State.updator => unit,
        array(ReasonReact.reactElement)
      ) =>
      ReasonReact.component(
        ReasonReact.stateless,
        ReasonReact.noRetainedProps,
        ReasonReact.actionless
      );
  };
