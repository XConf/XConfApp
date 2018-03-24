type route =
  | Home
  | Details(string);

let router = (route: route, ~utils: StackNavigator.routerUtils(route)) =>
  switch route {
  | Home => <HomeScreen onGoToDetailsPress=(() => utils.pushRoute(Details("first-page"))) />
  | Details(param) =>
    <DetailsScreen
      param
      onGoBackPress=utils.popRoute
      onGoToAnotherDetailsPress=((param) => utils.pushRoute(Details(param)))
    />
  };
