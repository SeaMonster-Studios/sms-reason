type date = {
  .
  [@bs.meth] "getUTCMonth": unit => int,
  [@bs.meth] "getUTCDate": unit => int,
  [@bs.meth] "getUTCFullYear": unit => int,
};

[@bs.new] external init: string => date = "Date";
[@bs.new] external initFloat: float => date = "Date";
[@bs.new] external initNow: unit => date = "Date";
[@bs.new] external now: unit => int = "Date";
[@bs.send] external toIsoString: date => string = "toISOString";
[@bs.send] external getTime: date => int = "getTime";

let getDateFileNameString = () => {
  let date = initNow();
  let month = (date##getUTCMonth() + 1)->string_of_int; // months from 0-11
  let day = date##getUTCDate()->string_of_int;
  let year = date##getUTCFullYear()->string_of_int;

  year ++ "/" ++ month ++ "/" ++ day;
};