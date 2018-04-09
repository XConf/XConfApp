external castResponse : string => {. "data": Js.Json.t} = "%identity";

external asJsObject : 'a => Js.t({..}) = "%identity";

[@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

[@bs.module]
external shallowEqual : (Js.t({..}), Js.t({..})) => bool =
  "fbjs/lib/shallowEqual";

type response =
  | Loading
  | Loaded(Js.Json.t)
  | Failed(string);

type state = {
  response,
  variables: Js.Json.t,
  fetching: bool,
  error: option(string),
};

type action =
  | Result(string)
  | Error(string)
  | Refetch;

let a = [%bs.raw {| 1 |}];

type utils('a) = {
  parse: 'a,
  refetch: unit => unit,
  fetching: bool,
  error: option(string),
};

let makeQueryObject: (ApolloClient.queryObj, 'a) => ApolloClient.queryObj = [%bs.raw
  {|
  (a, b) => ({ ...a, ...b })
|}
];

let sendQuery = (~query, ~reduce, ~fetchPolicy="cache-first", ()) => {
  let queryObj =
    makeQueryObject(
      {"query": gql(. query##query), "variables": query##variables},
      {"fetchPolicy": fetchPolicy},
    );
  Js.Promise.(
    resolve(Apollo.Client.apolloClient##query(queryObj))
    |> then_(value => {
         reduce(() => Result(value), ());
         resolve();
       })
    |> catch(_value => {
         reduce(() => Error("an error happened"), ());
         resolve();
       })
  );
  ();
};

let refetch = (~query, ~reduce) => {
  reduce(() => Refetch, ());
  sendQuery(~query, ~reduce, ~fetchPolicy="network-only");
};

let component = ReasonReact.reducerComponent("Query");

let make = (~query as q, children) => {
  ...component,
  initialState: () => {
    response: Loading,
    variables: q##variables,
    fetching: true,
    error: None,
  },
  reducer: (action, state) =>
    switch (action) {
    | Result(result) =>
      let typedResult = castResponse(result)##data;
      ReasonReact.Update({
        ...state,
        response: Loaded(typedResult),
        fetching: false,
      });
    | Error(error) =>
      switch (state.response) {
      | Loaded(_) =>
        ReasonReact.Update({...state, fetching: false, error: Some(error)})
      | _ =>
        ReasonReact.Update({
          ...state,
          response: Failed(error),
          fetching: false,
          error: Some(error),
        })
      }
    | Refetch => ReasonReact.Update({...state, fetching: true, error: None})
    },
  willReceiveProps: ({state, reduce}) =>
    if (!
          shallowEqual(
            asJsObject(q##variables),
            asJsObject(state.variables),
          )) {
      sendQuery(~query=q, ~reduce);
      {...state, variables: q##variables};
    } else {
      state;
    },
  didMount: (_) =>
    ReasonReact.SideEffects(
      ({reduce}) => sendQuery(~query=q, ~reduce, ()),
    ),
  render: ({state, reduce}) => {
    let utils = {
      parse: q##parse,
      refetch: () => refetch(~query=q, ~reduce, ()),
      error: state.error,
      fetching: state.fetching,
    };
    children(state.response, utils);
  },
};
