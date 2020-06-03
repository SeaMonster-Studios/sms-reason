[@bs.deriving abstract]
type options = {
  [@bs.optional]
  path: string,
  [@bs.optional]
  expires: float,
  [@bs.optional]
  domain: string,
  [@bs.optional]
  secure: bool,
};

[@bs.module "js-cookie"]
external set:
  (~name: string, ~value: string, ~options: options=?, unit) => unit =
  "set";

[@bs.module "js-cookie"]
external get: string => Js.Nullable.t(string) = "get";

[@bs.module "js-cookie"]
external getJson: string => Js.Nullable.t(Js.Json.t) = "getJSON";

[@bs.module "js-cookie"] external remove: string => unit = "remove";