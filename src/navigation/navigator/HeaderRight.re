[@bs.module "./HeaderRightJS"] external jsHeaderRight : ReasonReact.reactClass = "default";

let make = (children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=jsHeaderRight,
    ~props=Js.Obj.empty(),
    children
  );
