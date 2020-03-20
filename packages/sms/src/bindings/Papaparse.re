type parsed = {
  data: Js.Json.t,
  errors: array(string),
};

external jsonToJsOb: Js.Json.t => Js.t('a) = "%identity";
type error = {
  type_: string,
  code: string,
  message: string,
  row: int,
};
type config('file, 'inputElem) = {
  download: bool,
  quotes: bool,
  quoteChar: string,
  escapeChar: string,
  delimiter: string,
  header: bool,
  newline: string,
  skipEmptyLines: bool,
  columns: Js.Nullable.t(array(string)),
  complete: parsed => unit,
  error: (error, 'file, 'inputElem, string) => unit,
  before: ('file, 'inputElem) => unit,
};
let defaultConfig = {
  download: false,
  quotes: false,
  quoteChar: "\"",
  escapeChar: "\"",
  delimiter: ",",
  header: false,
  newline: "\n",
  skipEmptyLines: false,
  columns: Js.Nullable.null,
  complete: _ => (),
  error: (_, _, _, _) => (),
  before: (_, _) => (),
};
[@bs.module "papaparse"]
external parse: ('a, config('file, 'inputElem)) => unit = "parse";