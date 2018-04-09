type route =
  | Map
  | Info;

let router = (route: route, ~utils: ModalStackNavigator.routerUtils(route)) =>
  switch route {
  | Map =>
    <MapScreen
      onClosePress=utils.popRoute
    />
  | Info =>
    <InfoScreen
      onOpenAnotherModalPress=(() => utils.pushRoute(Info))
      onClosePress=utils.popRoute
    />
  };
