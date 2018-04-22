type route =
  | Schedule
  | ScheduleItem(NavigationTypes.scheduleItemData)
  | Information
  | Details(string);

type screenEvent =
  | OpenMap;

let router = (route: route, ~utils: StackNavigator.routerUtils(route, screenEvent)) =>
  switch route {
  | Schedule =>
    <ScheduleScreen
      onMapPress=(() => utils.sendEvent(OpenMap))
      onScheduleItemPress=(scheduleItem => utils.pushRoute(ScheduleItem({
        id: scheduleItem##id,
        eventTypeName: switch (scheduleItem##eventInterface) {
        | `Session(_) => "Session"
        | `Activity(_) => "Activity"
        },
        eventTitle: scheduleItem##event##title
      })))
    />
  | ScheduleItem(scheduleItemData) =>
    <ScheduleItemScreen scheduleItemData />
  | Information =>
    <InformationScreen />
  | Details(param) =>
    <DetailsScreen
      param
      onGoBackPress=utils.popRoute
      onGoToAnotherDetailsPress=((param) => utils.pushRoute(Details(param)))
    />
  };
