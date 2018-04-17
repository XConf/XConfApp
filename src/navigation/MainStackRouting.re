type route =
  | Schedule
  | ScheduleItem({. "id": string, "date": {. "name": string}, "event": {. "title": string}})
  | Information
  | Details(string);

type screenEvent =
  | OpenMap;

let router = (route: route, ~utils: StackNavigator.routerUtils(route, screenEvent)) =>
  switch route {
  | Schedule =>
    <ScheduleScreen
      onMapPress=(() => utils.sendEvent(OpenMap))
      onScheduleItemPress=((scheduleItem) => utils.pushRoute(ScheduleItem({"id": scheduleItem##id, "date": {"name": scheduleItem##date##name} , "event": {"title": scheduleItem##event##title}})))
    />
  | ScheduleItem(scheduleItem) =>
    <ScheduleItemScreen
      scheduleItem
    />
  | Information =>
    <InformationScreen />
  | Details(param) =>
    <DetailsScreen
      param
      onGoBackPress=utils.popRoute
      onGoToAnotherDetailsPress=((param) => utils.pushRoute(Details(param)))
    />
  };
