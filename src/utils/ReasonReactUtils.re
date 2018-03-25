let refToSelf = (r): ReasonReact.self('state, 'retainedProps, 'action) => {
  let jsObj = ReasonReact.refToJsObj(r);
  jsObj##self(jsObj##state)
};
