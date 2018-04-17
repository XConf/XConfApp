/*type state = {
    title: string
  };

  type action =
    | SetTitle(string);*/
/*let component = ReasonReact.reducerComponent("ScheduleItemScreen");*/
let component = ReasonReact.statelessComponent("ScheduleItemScreen");

let make = (~scheduleItem, _children) => {
  ...component,
  /*initialState: () => {title: "Schedule"},
    reducer: (action, state) =>
      switch action {
      | SetTitle(title) => ReasonReact.Update({...state, title})
      },*/
  render: self => {
    let dateName = scheduleItem##date##name;
    <View style=Style.(style([flex(1.)]))>
      <StackNavigator.Header style="default">
        <StackNavigator.Header.TitleText value={j|$dateName Event|j} />
      </StackNavigator.Header>
      <ScheduleItemContainer scheduleItem />
    </View>;
  },
};
