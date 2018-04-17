let dateFnsFromGql: Js.Json.t => Js.Date.t = [%bs.raw {| Date.parse |}];
