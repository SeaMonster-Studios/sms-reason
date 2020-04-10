type errorDesc = string;
type token = string;
type tokenType = string;
type error = string;
type tokenReceivedCallback = (errorDesc, token, error, tokenType) => unit;
type scopes = array(string);

module Config = {
  [@bs.deriving abstract]
  type auth = {
    clientId: string,
    [@bs.optional]
    authority: string,
    [@bs.optional]
    validateAuthority: bool,
    redirectUri: unit => string,
    [@bs.optional]
    postLogoutRedirectUri: unit => string,
    [@bs.optional]
    navigateToLoginRequestUrl: bool,
  };

  [@bs.deriving abstract]
  type cache = {
    // options: localStorage  or localSession
    [@bs.optional]
    cacheLocation: string,
    [@bs.optional]
    storeAuthStateInCookie: bool,
  };

  type logger;
  type telemetry;

  [@bs.deriving abstract]
  type system = {
    [@bs.optional]
    logger,
    [@bs.optional]
    loadFrameTimeout: float,
    [@bs.optional]
    tokenRenewalOffsetSeconds: float,
    [@bs.optional]
    navigateFrameWait: float,
    [@bs.optional]
    telemetry,
  };

  type framework;

  [@bs.deriving abstract]
  type t = {
    auth,
    [@bs.optional]
    cache,
    [@bs.optional]
    system,
    [@bs.optional]
    framework,
  };
};

type claims = {
  exp: int,
  nbf: int,
  ver: string,
  iss: string,
  sub: string,
  aud: string,
  nonce: string,
  iat: int,
  auth_time: int,
  oid: string,
  name: string,
  jobTitle: string,
  emails: array(string),
  tfp: string,
};

type idToken = {
  rawIdToken: string,
  issuer: string,
  objectId: string,
  subject: string,
  version: string,
  name: string,
  nonce: string,
  expiration: int,
  claims,
  idTokenClaims: claims,
};

type account = {
  accountIdentifier: string,
  homeAccountIdentifier: string,
  userName: Js.Nullable.t(string),
  name: string,
  idToken,
  idTokenClaims: idToken,
  sid: Js.Nullable.t(string),
  environment: string,
};

type loginPopupValue = {
  idToken,
  idTokenClaims: idToken,
  uniqueId: string,
  tenantId: string,
  tokenType: string,
  accountState: string,
  fromCache: bool,
  scopes: array(string),
  accessToken: Js.Nullable.t(string),
};

[@bs.deriving abstract]
type authenticationParameters = {
  [@bs.optional]
  scopes: array(string),
  [@bs.optional]
  extraScopesToConsent: array(string),
  [@bs.optional]
  prompt: string,
  [@bs.optional]
  extraQueryPrameters: Js.Dict.t(string),
  [@bs.optional]
  claimsRequest: string,
  [@bs.optional]
  authority: string,
  [@bs.optional]
  state: string,
  [@bs.optional]
  correlationId: string,
  [@bs.optional]
  account,
  [@bs.optional]
  sid: string,
  [@bs.optional]
  loginHint: string,
  [@bs.optional]
  forceRefresh: bool,
  [@bs.optional]
  redirectUri: string,
  [@bs.optional]
  redirectStartPage: string,
};

type app = {
  .
  "authority": string,
  "clientId": string,
  "inCookie": bool,
  [@bs.meth] "logout": unit => unit,
  [@bs.meth] "getAccount": unit => Js.Nullable.t(account),
  [@bs.meth] "loginPopup": scopes => Js.Promise.t(loginPopupValue),
  [@bs.meth]
  "acquireTokenSilent":
    authenticationParameters => Js.Promise.t(loginPopupValue),
  [@bs.meth]
  "acquireTokenPopup":
    authenticationParameters => Js.Promise.t(loginPopupValue),
};

[@bs.module "msal"] [@bs.new]
external make: Config.t => app = "UserAgentApplication";

let%private isTruthy: account => bool = [%raw
  {|
  function(account) {
    return !!account
  }
|}
];

let getAccount = app => {
  switch (app##getAccount()->Js.Nullable.toOption) {
  | None => None
  | Some(account) =>
    /**Sometimes this isn't null but still a JS "falsey" value */
    (account->isTruthy)
      ? Some(account) : None
  };
};