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
    module State: {
      type routeEntry = {
        route: R.route,
        key: string,
        screenRef: ref(option(ReasonReact.reactRef))
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
    let initialStateWithRoute: R.route => State.t;
    let make:
      (
        ~state: State.t,
        ~updateState: State.updator => unit,
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
