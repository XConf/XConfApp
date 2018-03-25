type state;

type action;

let make:
  (~onGoToDetailsPress: unit => unit, array(ReasonReact.reactElement)) =>
  ReasonReact.component(state, ReasonReact.noRetainedProps, action);

let scrollToTop: ReasonReact.reactRef => unit;
