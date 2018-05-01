module ConferenceScheduleQuery = [%graphql {|
  query queryConferenceSchedule($conferenceCode: String!) {
    conference(code: $conferenceCode) {
      id
      name
      schedule {
        places {
          id
        }
        items {
          id
          event {
            id
            title
          }
          eventInterface @bsVariant {
            activity {
              id
            }
            session {
              id
              language
              tags {
                name
              }
            }
          }
          periods {
            id
            date {
              id
              name
              date @bsDecoder(fn: "Decoder.date")
            }
            start @bsDecoder(fn: "Decoder.date")
            end @bsDecoder(fn: "Decoder.date")
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

type t = ConferenceScheduleQuery.t;
type scheduleItem = 'a constraint t = {.. "conference": option({.. "schedule": {.. "items": array('a)}})};

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
