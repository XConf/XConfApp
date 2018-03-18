[@bs.module "./HeaderTitleTextJS"] external jsHeaderTitleText : ReasonReact.reactClass = "default";

let make = (~value: string, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=jsHeaderTitleText,
    ~props={"value": value},
    children
  );
