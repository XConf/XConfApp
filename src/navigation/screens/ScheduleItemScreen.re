let component = ReasonReact.statelessComponent("ScheduleItemScreen");

let eventTypeNameOfScheduleItem = scheduleItem =>
  switch (scheduleItem##eventInterface) {
  | `Activity(_) => "Activity"
  | `Session(_) => "Session"
  };

let make = (~scheduleItem, _children) => {
  ...component,
  render: _self =>
    <View style=Style.(style([flex(1.)]))>
      <StackNavigator.Header style="default">
        <StackNavigator.Header.TitleText
          value=(eventTypeNameOfScheduleItem(scheduleItem))
        />
      </StackNavigator.Header>
      <ScheduleItemContainer scheduleItemId=scheduleItem##id />
    </View>,
};
