type date = {
  .
  [@bs.meth] "getUTCMonth": unit => float,
  [@bs.meth] "getUTCDate": unit => float,
  [@bs.meth] "getUTCFullYear": unit => float,
  [@bs.meth] "setMonth": float => float,
  [@bs.meth] "getMonth": unit => float,
};

[@bs.new] external init: string => date = "Date";
[@bs.new] external initFloat: float => date = "Date";
[@bs.new] external initNow: unit => date = "Date";
[@bs.new] external now: unit => float = "Date";
[@bs.send] external toIsoString: date => string = "toISOString";
[@bs.send] external getTime: date => float = "getTime";

let getDateFileNameString = () => {
  let date = initNow();
  let month = (date##getUTCMonth() +. 1.)->Js.Float.toString; // months from 0-11
  let day = date##getUTCDate()->Js.Float.toString;
  let year = date##getUTCFullYear()->Js.Float.toString;

  year ++ "/" ++ month ++ "/" ++ day;
};