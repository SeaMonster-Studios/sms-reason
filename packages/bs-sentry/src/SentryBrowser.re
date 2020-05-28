open Sentry;

[@bs.module "@sentry/browser"]
external make: Js.Nullable.t(options) => unit = "init";

[@bs.module "@sentry/browser"]
external capturePromiseException: Js.Promise.error => unit =
  "captureException";

[@bs.module "@sentry/browser"]
external captureException: 'a => unit = "captureException";

[@bs.module "@sentry/browser"]
external captureMessage: string => unit = "captureMessage";

[@bs.module "@sentry/browser"]
external showReportDialog: unit => unit = "showReportDialog";

[@bs.module "@sentry/browser"]
external withScope: scope => unit = "showReportDialog";

let captureDeccoError = captureDeccoError(captureMessage);