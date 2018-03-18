[@bs.module "./ButtonJS.js"] external jsButton : ReasonReact.reactClass = "default";

let optBoolToOptJsBoolean =
  fun
  | None => None
  | Some(v) => Some(Js.Boolean.to_js_boolean(v));

let make =
    (
      ~accessibilityLabel=?,
      ~color=?,
      ~disabled=?,
      ~onPress=?,
      ~testID=?,
      ~title=?,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=jsButton,
    ~props=
      Js.Undefined.(
        {
          "accessibilityLabel": fromOption(accessibilityLabel),
          "color": fromOption(color),
          "disabled": fromOption(optBoolToOptJsBoolean(disabled)),
          "onPress": fromOption(onPress),
          "testID": fromOption(testID),
          "title": fromOption(title),
        }
      ),
  );
