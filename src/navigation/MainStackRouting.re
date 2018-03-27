type route =
  | Home
  | Details(string);

type screenEvent =
  | OpenInfo;

let router = (route: route, ~utils: StackNavigator.routerUtils(route, screenEvent)) =>
  switch route {
  | Home =>
    <HomeScreen
      onInfoPress=(() => utils.sendEvent(OpenInfo))
      onGoToDetailsPress=(() => utils.pushRoute(Details("first-page")))
    />
  | Details(param) =>
    <DetailsScreen
      param
      onGoBackPress=utils.popRoute
      onGoToAnotherDetailsPress=((param) => utils.pushRoute(Details(param)))
    />
  };
