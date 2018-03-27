type route =
  | Info;

let router = (route: route, ~utils: ModalStackNavigator.routerUtils(route)) =>
  switch route {
  | Info =>
    <InfoScreen
      onOpenAnotherModalPress=(() => utils.pushRoute(Info))
      onClosePress=utils.popRoute
    />
  };
