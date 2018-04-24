[@bs.module "./Decoder"] external jsDate : 'a => Js.Date.t = "date";
[@bs.module "./Decoder"] external jsUrl : 'a => string = "url";

let date: 'a => Js.Date.t = jsDate;
let url: 'a => string = jsUrl;
