/*type state = {
    title: string
  };

  type action =
    | SetTitle(string);*/
/*let component = ReasonReact.reducerComponent("ScheduleItemScreen");*/
let component = ReasonReact.statelessComponent("ScheduleItemScreen");

let make = (~scheduleItemData, _children) => {
  ...component,
  /*initialState: () => {title: "Schedule"},
    reducer: (action, state) =>
      switch action {
      | SetTitle(title) => ReasonReact.Update({...state, title})
      },*/
  render: self => {
    let eventTypeName = NavigationTypes.(scheduleItemData.eventTypeName);
    <View style=Style.(style([flex(1.)]))>
      <StackNavigator.Header style="default">
        <StackNavigator.Header.TitleText value=eventTypeName />
      </StackNavigator.Header>
      <ScheduleItemContainer scheduleItemId=scheduleItemData.id />
    </View>;
  },
};
