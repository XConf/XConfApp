[@bs.module "./SectionScrollView"]
external jsSectionScrollView : ReasonReact.reactClass = "default";

[@bs.module "./SectionScrollView"]
external jsSectionScrollViewHeader : ReasonReact.reactClass = "Header";

[@bs.module "./SectionScrollView"]
external jsScrollToTop: (ReasonReact.reactRef) => unit = "scrollToTop";

let make =
    (
      ~ref=?,
      ~style=?,
      ~refreshControl: option(ReasonReact.reactElement)=?,
      children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=jsSectionScrollView,
    ~props=
      Js.Undefined.(
        {
          "componentRef": fromOption(ref),
          "style": fromOption(style),
          "refreshControl": fromOption(refreshControl),
        }
      ),
    children,
  );

module Section = {
  let make = (~header: ReasonReact.reactElement, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=jsSectionScrollView,
      ~props={"header": header},
      children,
    );
};

let scrollToTop: (ReasonReact.reactRef) => unit = jsScrollToTop;
