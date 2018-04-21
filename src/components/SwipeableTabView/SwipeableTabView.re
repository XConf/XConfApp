[@bs.module "./SwipeableTabView"] external jsSwipeableTabView : ReasonReact.reactClass = "default";

let make = (children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=jsSwipeableTabView,
    ~props=Js.Obj.empty(),
    children
  );

module Tab = {
  [@bs.module "./SwipeableTabView"] external jsTab : ReasonReact.reactClass = "Tab";
  let make = (~title: string, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=jsTab,
      ~props={"title": title},
      children
    );
}
