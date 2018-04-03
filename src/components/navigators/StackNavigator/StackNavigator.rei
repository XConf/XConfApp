type routerUtils('route, 'event) = {
  pushRoute: 'route => unit,
  popRoute: unit => unit,
  sendEvent: 'event => unit
};

module type Routing = {
  type route;
  type screenEvent;
  let router: (route, ~utils: routerUtils(route, screenEvent)) => ReasonReact.reactElement;
};

module Make:
  (R: Routing) =>
  {
    type routeEntry = {
      route: R.route,
      key: string,
      screenRef: ref(option(ReasonReact.reactRef))
    };
    type navigationState = {
      index: int,
      routes: list(routeEntry)
    };
    let initialStateWithRoute: R.route => navigationState;
    let routePushed: (R.route, navigationState) => navigationState;
    let routePoped: navigationState => navigationState;
    let routePopToToped: navigationState => navigationState;
    let make:
      (
        ~state: navigationState,
        ~updateState: navigationState => unit,
        ~handleEvent: R.screenEvent => unit=?,
        array(ReasonReact.reactElement)
      ) =>
      ReasonReact.component(
        ReasonReact.stateless,
        ReasonReact.noRetainedProps,
        ReasonReact.actionless
      );
  };

module Header: {
  let make:
    (~style: string, array(ReasonReact.reactElement)) =>
    ReasonReact.component(
      ReasonReact.stateless,
      ReasonReact.noRetainedProps,
      ReasonReact.actionless
    );
  module TitleText: {
    let make:
      (~value: string, array(ReasonReact.reactElement)) =>
      ReasonReact.component(
        ReasonReact.stateless,
        ReasonReact.noRetainedProps,
        ReasonReact.actionless
      );
  };
  module Right: {
    let make:
      array(ReasonReact.reactElement) =>
      ReasonReact.component(
        ReasonReact.stateless,
        ReasonReact.noRetainedProps,
        ReasonReact.actionless
      );
  };
  module Left: {
    let make:
      array(ReasonReact.reactElement) =>
      ReasonReact.component(
        ReasonReact.stateless,
        ReasonReact.noRetainedProps,
        ReasonReact.actionless
      );
  };
};
