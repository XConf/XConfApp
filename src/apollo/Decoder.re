[@bs.module "./Decoder"]
external jsDateFnsFromGql : Js.Json.t => Js.Date.t = "dateFnsFromGql";

let dateFnsFromGql: Js.Json.t => Js.Date.t = jsDateFnsFromGql;
