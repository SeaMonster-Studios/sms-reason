type tz = {. [@bs.meth] "names": unit => array(string)};

type moment = {
  .
  [@bs.meth] "tz": string => moment,
  [@bs.meth] "format": string => string,
  [@bs.meth] "fromNow": unit => string,
};

[@bs.module "moment-timezone"] external tz: tz = "tz";
[@bs.module] external moment: string => moment = "moment-timezone";