[@bs.module "./Decoder"]
external jsDate : 'a => Js.Date.t = "date";

let date: 'a => Js.Date.t = jsDate;
