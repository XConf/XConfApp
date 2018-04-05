[@bs.module "./StackNavigatorJS"] external jsStackNavigator : ReasonReact.reactClass = "default";

[@bs.module "./StackNavigatorJS"] external jsStackNavigatorHeader : ReasonReact.reactClass =
  "Header";

[@bs.module "./StackNavigatorJS"]
external jsStackNavigatorHeaderTitleText : ReasonReact.reactClass =
  "HeaderTitleText";

[@bs.module "./StackNavigatorJS"] external jsStackNavigatorHeaderRight : ReasonReact.reactClass =
  "HeaderRight";

[@bs.module "./StackNavigatorJS"] external jsStackNavigatorHeaderLeft : ReasonReact.reactClass =
  "HeaderLeft";

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

let randomKey = () : string => string_of_int(Random.bits()) ++ "-" ++ string_of_int(Random.bits());

module Make = (R: Routing) => {
  module State = {
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
    let routePushed = (route: R.route, state: t) : t => {
      index: state.index + 1,
      routes: [{key: randomKey(), route, screenRef: ref(None)}, ...state.routes]
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
      | 0 => state
      | _ => {index: 0, routes: onlyLast(state.routes)}
      };
  };
  let initialStateWithRoute = (route: R.route) : State.t => {
    index: 0,
    routes: [{key: randomKey(), route, screenRef: ref(None)}]
  };
  let getRouterUtilsByUpdateStateAndHandleEvent = (updateState, handleEvent) => {
    pushRoute: (route: R.route) => State.routePushed(route) |> updateState,
    popRoute: () => State.routePoped |> updateState,
    sendEvent: handleEvent
  };
  let getUtilsAppliedRouter = (utils) => R.router(~utils);
  let make =
      (
        ~state: State.t,
        ~updateState: State.updator => unit,
        ~handleEvent: R.screenEvent => unit=(_) => (),
        children
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=jsStackNavigator,
      ~props={
        "router": R.router,
        "state": state,
        "updateState": updateState,
        "handleEvent": handleEvent,
        "getRouterUtilsByUpdateStateAndHandleEvent": getRouterUtilsByUpdateStateAndHandleEvent,
        "getUtilsAppliedRouter": getUtilsAppliedRouter
      },
      children
    );
};

module Header = {
  let make = (~style: string, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=jsStackNavigatorHeader,
      ~props={"style": style},
      children
    );
  module TitleText = {
    let make = (~value: string, children) =>
      ReasonReact.wrapJsForReason(
        ~reactClass=jsStackNavigatorHeaderTitleText,
        ~props={"value": value},
        children
      );
  };
  module Right = {
    let make = (children) =>
      ReasonReact.wrapJsForReason(
        ~reactClass=jsStackNavigatorHeaderRight,
        ~props=Js.Obj.empty(),
        children
      );
  };
  module Left = {
    let make = (children) =>
      ReasonReact.wrapJsForReason(
        ~reactClass=jsStackNavigatorHeaderLeft,
        ~props=Js.Obj.empty(),
        children
      );
  };
};
