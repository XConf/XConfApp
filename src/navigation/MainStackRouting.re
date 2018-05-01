type route =
  | Schedule
  | ScheduleItem(ScheduleScreen.scheduleItem)
  | Information
  | Details(string);

type screenEvent =
  | OpenMap;

let router = (route: route, ~utils: StackNavigator.routerUtils(route, screenEvent)) =>
  switch route {
  | Schedule =>
    <ScheduleScreen
      onMapPress=(() => utils.sendEvent(OpenMap))
      onScheduleItemPress=(scheduleItem => utils.pushRoute(ScheduleItem(scheduleItem)))
    />
  | ScheduleItem(scheduleItem) =>
    <ScheduleItemScreen scheduleItem />
  | Information =>
    <InformationScreen />
  | Details(param) =>
    <DetailsScreen
      param
      onGoBackPress=utils.popRoute
      onGoToAnotherDetailsPress=((param) => utils.pushRoute(Details(param)))
    />
  };
