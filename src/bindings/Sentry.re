type scope = {. [@bs.meth] "setExtra": (string, string) => unit};

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

let captureDeccoError = (error: Decco.decodeError, decoderName: string) => {
  let path = error.path;
  let message = error.message;
  let value = error.value->Js.Json.stringify;
  let report = {j|
    DECODE ERROR WITH $decoderName
    Path: $path

    Message: $message

    Value: $value
  |j};
  Js.log(report);
  captureException(report);
};