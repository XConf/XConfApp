type route =
  | Home
  | Details(string);

let router = (route: route, ~pushRoute, ~popRoute) =>
  switch route {
  | Home => <HomeScreen onGoToDetailsPress=(() => pushRoute(Details("first-page"))) />
  | Details(param) =>
    <DetailsScreen
      param
      onGoBackPress=popRoute
      onGoToAnotherDetailsPress=((param) => pushRoute(Details(param)))
    />
  };
