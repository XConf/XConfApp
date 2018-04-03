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

let routesListFromArray = (array) => Array.fold_left((list, item) => [item, ...list], [], array);

let routesArrayFromList = (list) =>
  List.fold_left((array, item) => Array.append([|item|], array), [||], list);

module Make = (R: Routing) => {
  type routeEntry = {
    route: R.route,
    key: string,
    screenRef: ref(option(ReasonReact.reactRef))
  };
  type navigationState = {
    index: int,
    routes: list(routeEntry)
  };
  type jsRouteEntry = {
    .
    "route": R.route, "key": string, "screenRef": ref(option(ReasonReact.reactRef))
  };
  type jsNavigationState = {. "index": int, "routes": array(jsRouteEntry)};
  let routeEntryFromJsRouteEntry: jsRouteEntry => routeEntry =
    (jsRouteEntry) => {
      route: jsRouteEntry##route,
      key: jsRouteEntry##key,
      screenRef: jsRouteEntry##screenRef
    };
  let stateFromJsState: jsNavigationState => navigationState =
    (jsNavigationState) => {
      let jsRoutesArray = jsNavigationState##routes;
      let routesArray = Array.map(routeEntryFromJsRouteEntry, jsRoutesArray);
      let routesList = routesListFromArray(routesArray);
      {index: jsNavigationState##index, routes: routesList}
    };
  let jsRouteEntryFromRouteEntry: routeEntry => jsRouteEntry =
    (routeEntry) => {
      "route": routeEntry.route,
      "key": routeEntry.key,
      "screenRef": routeEntry.screenRef
    };
  let jsStateFromState: navigationState => jsNavigationState =
    (navigationState) => {
      let routesList = navigationState.routes;
      let routesArray = routesArrayFromList(routesList);
      let jsRoutesArray = Array.map(jsRouteEntryFromRouteEntry, routesArray);
      {"index": navigationState.index, "routes": jsRoutesArray}
    };
  let random = () : string => string_of_int(Random.bits()) ++ "-" ++ string_of_int(Random.bits());
  let initialStateWithRoute = (route: R.route) : navigationState => {
    index: 0,
    routes: [{key: random(), route, screenRef: ref(None)}]
  };
  let routePushed = (route: R.route, state: navigationState) : navigationState => {
    index: state.index + 1,
    routes: [{key: random(), route, screenRef: ref(None)}, ...state.routes]
  };
  let routePoped = (state: navigationState) : navigationState =>
    switch (state.index, state.routes) {
    | (0, _) => state
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
    | 0 => state
    | _ => {index: 0, routes: onlyLast(state.routes)}
    };
  let make =
      (
        ~state: navigationState,
        ~updateState: navigationState => unit,
        ~handleEvent: R.screenEvent => unit=(_) => (),
        children
      ) => {
    let routerUtils = {
      pushRoute: (route: R.route) => routePushed(route, state) |> updateState,
      popRoute: () => routePoped(state) |> updateState,
      sendEvent: handleEvent
    };
    ReasonReact.wrapJsForReason(
      ~reactClass=jsStackNavigator,
      ~props={
        "router": R.router(~utils=routerUtils),
        "state": jsStateFromState(state),
        "updateState": (jsNavigationState) => updateState(stateFromJsState(jsNavigationState))
      },
      children
    )
  };
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
