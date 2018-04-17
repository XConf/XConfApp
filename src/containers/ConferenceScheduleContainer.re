module ConferenceScheduleQuery = [%graphql {|
  query queryConferenceSchedule($conferenceCode: String!) {
    conference(code: $conferenceCode) {
      id
      name
      schedule {
        items {
          id
          date {
            name
          }
          event {
            id
            title
          }
          periods {
            id
            start
            end
          }
          places {
            id
            name
          }
        }
      }
    }
  }
|}];

let component = ReasonReact.statelessComponent("ConferenceScheduleContainer");

let make = (~conferenceCode, ~componentRef, ~onScheduleItemPress, _children) => {
  ...component,
  render: _self => {
    let conferenceScheduleQuery =
      ConferenceScheduleQuery.make(~conferenceCode, ());
    <Query query=conferenceScheduleQuery>
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
               switch (result##conference) {
               | Some(conference) =>
                 <Schedule
                   ref=componentRef
                   schedule=conference##schedule
                   onItemPress=onScheduleItemPress
                   onRefresh=refetch
                   refreshing=fetching
                 />
               | None =>
                 <Text value={j|No such conference code: $conferenceCode|j} />
               }
             }
         )
    </Query>;
  }
};
