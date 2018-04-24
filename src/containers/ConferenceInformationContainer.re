module ConferenceInformationQuery = [%graphql
  {|
  query queryConferenceInformation($conferenceCode: String!) {
    conference(code: $conferenceCode) {
      id
      name
      keyVisionUrl @bsDecoder(fn: "Decoder.url")
      dates {
        date @bsDecoder(fn: "Decoder.date")
      }
      location {
        name
        address
      }
      wifiNetwork {
        ssid
        password
      }
    }
  }
|}
];

let component = ReasonReact.statelessComponent("ConferenceInformationContainer");

let make = (~conferenceCode, ~componentRef, _children) => {
  ...component,
  render: _self => {
    let conferenceInformationQuery =
      ConferenceInformationQuery.make(~conferenceCode, ());
    <Query query=conferenceInformationQuery>
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
                 <ConferenceInformation
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
