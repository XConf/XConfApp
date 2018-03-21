[@bs.module "./StackNavigatorHeaderJS"] external jsStackNavigatorHeader : ReasonReact.reactClass = "default";
[@bs.module "./StackNavigatorHeaderJS"] external jsTitleText : ReasonReact.reactClass = "TitleText";
[@bs.module "./StackNavigatorHeaderJS"] external jsHeaderRight : ReasonReact.reactClass = "HeaderRight";

let make = (~style: string, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=jsStackNavigatorHeader,
    ~props={"style": style},
    children
  );

module TitleText {
  let make = (~value: string, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=jsTitleText,
      ~props={"value": value},
      children
    );
};

module HeaderRight {
  let make = (children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=jsHeaderRight,
      ~props=Js.Obj.empty(),
      children
    );
};
