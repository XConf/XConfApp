module ScheduleItemQuery = [%graphql
  {|
  query queryScheduleItem($scheduleItemId: ID!) {
    scheduleItem(id: $scheduleItemId) {
      id
      eventInterface @bsVariant {
        activity {
          id
          title
        }
        session {
          id
          title
          description
          speaker {
            id
            name
          }
        }
      }
    }
  }
|}
];

let component = ReasonReact.statelessComponent("ScheduleItemContainer");

let make = (~scheduleItem, _children) => {
  ...component,
  render: _self => {
    let scheduleItemQuery = ScheduleItemQuery.make(~scheduleItemId=scheduleItem##id, ());
    <Query query=scheduleItemQuery>
      ...(
           (response, {refetch, fetching}) =>
             switch (response) {
             | Loading =>
               <View
                 style=Style.(
                         style([
                           flex(1.),
                           justifyContent(Center),
                           alignItems(Center),
                         ])
                       )>
                 <Text value="Loading..." />
               </View>
             | Failed(error) =>
               <View
                 style=Style.(
                         style([
                           flex(1.),
                           justifyContent(Center),
                           alignItems(Center),
                         ])
                       )>
                 <Text value={j|Error: $error|j} />
               </View>
             | Loaded(result) =>
               switch (result##scheduleItem) {
               | Some(scheduleItem) =>
                 <ScheduleItemDetails
                   scheduleItem
                   onRefresh=refetch
                   refreshing=fetching
                 />
               | None =>
                 <Text value={j|No such schedule item.|j} />
               }
             }
         )
    </Query>;
  },
};
