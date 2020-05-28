open Sentry;

[@bs.module "@sentry/node"]
external make: Js.Nullable.t(options) => unit = "init";

[@bs.module "@sentry/node"]
external capturePromiseException: Js.Promise.error => unit =
  "captureException";

[@bs.module "@sentry/node"]
external captureException: 'a => unit = "captureException";

[@bs.module "@sentry/node"]
external captureMessage: string => unit = "captureMessage";

[@bs.module "@sentry/node"]
external withScope: scope => unit = "showReportDialog";

let captureDeccoError = captureDeccoError(captureMessage);