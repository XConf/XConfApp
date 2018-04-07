module type Config = {type tab; let numberOfTabs: int;};

module Make:
  (C: Config) =>
  {
    type tab = C.tab;
    module State: {
      type t = {activeTab: tab};
      type updator = t => t;
    };
    let initialStateWithDefaultTab: tab => State.t;
    type tabBarOnPressPayload = {
      activedTab: tab,
      previousActiveTab: option(tab)
    };
    let make:
      (
        ~state: State.t,
        ~updateState: State.updator => unit,
        array(ReasonReact.reactElement)
      ) =>
      ReasonReact.component(
        ReasonReact.stateless,
        ReasonReact.noRetainedProps,
        ReasonReact.actionless
      );
    module Tab: {
      let make:
        (
          ~title: string,
          ~tabBarIcon: (~focused: bool, ~tintColor: string) =>
                       ReasonReact.reactElement,
          ~tabBarOnPress: tabBarOnPressPayload => unit=?,
          array(ReasonReact.reactElement)
        ) =>
        ReasonReact.component(
          ReasonReact.stateless,
          ReasonReact.noRetainedProps,
          ReasonReact.actionless
        );
    };
  };
