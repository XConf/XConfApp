module ConferenceInformationQuery = [%graphql
  {|
  query queryConferenceInformation($conferenceCode: String!) {
    conference(code: $conferenceCode) {
      name
    }
  }
|}
];

let component = ReasonReact.statelessComponent("InformationContainer");

let make = (~conferenceCode, ~componentRef, _children) => {
  ...component,
  render: _self => {
    let conferenceInformationQuery =
      ConferenceInformationQuery.make(~conferenceCode, ());
    <Query query=conferenceInformationQuery>
      ...(
           (response, {parse, refetch, fetching}) =>
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
               switch (parse(result)##conference) {
               | Some(conference) =>
                 <Information
                   ref=componentRef
                   conference
                   onRefresh=refetch
                   refreshing=fetching
                 />
               | None =>
                 <Text value={j|No such conference code: $conferenceCode|j} />
               }
             }
         )
    </Query>;
  },
};
