type scope = {. [@bs.meth] "setExtra": (string, string) => unit};

type event;
type eventHint;
type breadcrumb;
type breadcrumbHint;
type integration;
type transport;

[@bs.deriving abstract]
type options = {
  [@bs.optional]
  blacklistUrls: array(Js.Re.t),
  [@bs.optional]
  whitelistUrls: array(Js.Re.t),
  [@bs.optional]
  debug: bool,
  [@bs.optional]
  enabled: bool,
  [@bs.optional]
  dsn: string,
  [@bs.optional]
  defaultIntegrations: integration,
  [@bs.optional]
  integrations: array(integration),
  [@bs.optional]
  ignoreErrors: array(Js.Re.t),
  [@bs.optional]
  transport,
  [@bs.optional]
  transportOptions: Js.Dict.t(string),
  [@bs.optional]
  release: string,
  [@bs.optional]
  environment: string,
  [@bs.optional]
  dist: string,
  [@bs.optional]
  maxBreadcrumbs: int,
  [@bs.optional]
  logLevel: int,
  [@bs.optional]
  sampelRage: int,
  [@bs.optional]
  attachStacktrace: bool,
  [@bs.optional]
  beforeSend:
    (~event: event, ~hint: eventHint=?) => Js.Promise.t(Js.Nullable.t(event)),
  [@bs.optional]
  beforeBreadcrumb:
    (~breadcrumb: breadcrumb, ~hint: breadcrumbHint=?) =>
    Js.Promise.t(Js.Nullable.t(event)),
};

let captureDeccoError =
    (captureMessage, error: Decco.decodeError, decoderName: string) => {
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
  captureMessage(report);
};

module Integrations = {
  module GlobalHandlers = {
    [@bs.deriving abstract]
    type options = {
      [@bs.optional]
      onerror: bool,
      [@bs.optional]
      onunhandledrejection: bool,
    };
    [@bs.module "@sentry/browser"] [@bs.scope "Integrations"] [@bs.new]
    external make: options => integration = "GlobalHandlers";
  };
};
