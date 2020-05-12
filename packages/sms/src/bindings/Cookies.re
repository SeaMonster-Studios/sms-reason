[@bs.deriving abstract]
type setOptions = {
  [@bs.optional]
  path: string,
  [@bs.optional]
  expires: float,
  [@bs.optional]
  domain: string,
  [@bs.optional]
  secure: bool,
};

[@bs.module "js-cookie"] external set: (string, string) => unit = "set";

[@bs.module "js-cookie"]
external setWithOptions: (string, string, setOptions) => unit = "set";

[@bs.module "js-cookie"] external setJson: (string, Js.Json.t) => unit = "set";

[@bs.module "js-cookie"]
external setJsonWithOptions: (string, Js.Json.t, setOptions) => unit = "set";

[@bs.module "js-cookie"]
external get: string => Js.Nullable.t(string) = "get";

[@bs.module "js-cookie"]
external getJson: string => Js.Nullable.t(Js.Json.t) = "getJSON";

[@bs.module "js-cookie"] external remove: string => unit = "remove";