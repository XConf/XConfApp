[@bs.module "./Query"] external jsQuery : ReasonReact.reactClass = "default";

type response('a) =
  | Loading
  | Loaded('a)
  | Failed(string);

type utils = {
  refetch: unit => unit,
  fetching: bool,
  error: option(string),
};

let jsStateToResponse = (~parse, jsState) => {
  let fetching = Js.to_bool(jsState##fetching);
  let data = Js.Nullable.toOption(jsState##data);
  switch (fetching, data) {
  | (_, Some(d)) => Loaded(parse(d))
  | (true, None) => Loading
  | (false, None) =>
    let errorMessage = Js.Nullable.toOption(jsState##errorMessage);
    switch (errorMessage) {
    | None => Failed("Unknown error")
    | Some(m) => Failed(m)
    };
  };
};

let jsStateAndUtilsToUtils = (jsState, jsUtils) => ({
  refetch: jsUtils##refetch,
  fetching: Js.to_bool(jsState##fetching),
  error: Js.Nullable.toOption(jsState##errorMessage)
});

let make =
    (
      ~query,
      children: (response('a), utils) => ReasonReact.reactElement,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=jsQuery,
    ~props={
      "apolloClient": Apollo.Client.apolloClient,
      "query": query
    },
    (jsState, jsUtils) => {
      let response = jsStateToResponse(~parse=query##parse, jsState);
      let utils = jsStateAndUtilsToUtils(jsState, jsUtils);

      children(response, utils);
    }
  );
