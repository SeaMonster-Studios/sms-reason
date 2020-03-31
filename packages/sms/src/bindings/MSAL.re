type errorDesc = string;
type token = string;
type tokenType = string;
type error = string;
type tokenReceivedCallback = (errorDesc, token, error, tokenType) => unit;
type scopes = array(string);

[@bs.deriving abstract]
type config = {
  [@bs.optional]
  // options: localStorage  or localSession
  cacheLocation: string,
  [@bs.optional]
  postLogoutRedirectUri: string,
  [@bs.optional]
  redirectUri: string,
  [@bs.optional]
  storeAuthStateInCookie: bool,
  [@bs.optional]
  validateAuthority: bool,
};

type authorityInstance;
[@bs.get]
external aadInstanceDiscoveryEndpointUrl: authorityInstance => string =
  "AadInstanceDiscoveryEndpointUrl";
[@bs.get]
external authorityType: authorityInstance => string = "AuthorityType";
[@bs.get]
external authorizationEndpoint: authorityInstance => string =
  "AuthorizationEndpoint";
[@bs.get]
external canonicalAuthority: authorityInstance => string =
  "CanonicalAuthority";
[@bs.get]
external canonicalAuthorityUrlComponents: authorityInstance => string =
  "CanonicalAuthorityUrlComponents";
[@bs.get]
external defaultOpenIdConfigurationEndpoint: authorityInstance => string =
  "DefaultOpenIdConfigurationEndpoint";
[@bs.get]
external endSessionEndpoint: authorityInstance => string =
  "EndSessionEndpoint";
[@bs.get]
external isValidationEnabled: authorityInstance => bool =
  "IsValidationEnabled";
[@bs.get]
external selfSignedJwtAudience: authorityInstance => string =
  "SelfSignedJwtAudience";
[@bs.get] external tenant: authorityInstance => string = "Tenant";

type cacheLocations;
[@bs.get] external localStorage: cacheLocations => string = "localStorage";
[@bs.get] external sessionStorage: cacheLocations => string = "sessionStorage";

type cacheStorage;
[@bs.get]
external localStorageSupported: cacheStorage => bool =
  "_localStorageSupported";
[@bs.get]
external sessionStorageSupported: cacheStorage => bool =
  "_sessionStorageSupported";

type logger;
[@bs.get] external level: logger => int = "_level";
[@bs.get] external correlationId: logger => int = "_correlationId";
[@bs.get] external localCallback: logger => unit = "_localCallback";
[@bs.get] external piiLoggingEnabled: logger => unit = "_piiLoggingEnabled";

type app = {clientId: string};
[@bs.get] external clientId: app => string = "clientId";
[@bs.get] external authority: app => string = "authority";
[@bs.get]
external authorityInstance: app => authorityInstance = "authorityInstance";
[@bs.get] external cacheLocation: app => string = "cacheLocation";
[@bs.get] external loadFrameTimeout: app => int = "loadFrameTimeout";
[@bs.get]
external storeAuthStateInCookie: app => bool = "storeAuthStateInCookie";
[@bs.get] external validateAuthority: app => bool = "validateAuthority";
[@bs.get] external cacheLocations: app => cacheLocations = "_cacheLocations";
[@bs.get] external cacheStorage: app => cacheStorage = "_cacheStorage";
[@bs.get] external logger: app => logger = "_logger";
[@bs.get]
external acquireTokenInProgress: app => bool = "_acquireTokenInProgress";
[@bs.get] external clockSkew: app => int = "_clockSkew";
[@bs.get] external isAngular: app => bool = "_isAngular";
[@bs.get] external loginInProgress: app => bool = "_loginInProgress";
[@bs.get]
external navigateToLoginRequestUrl: app => bool = "_navigateToLoginRequestUrl";
[@bs.get]
external postLogoutredirectUri: app => string = "_postLogoutredirectUri";
[@bs.get] external redirectUri: app => string = "_redirectUri";
[@bs.get] external state: app => string = "_state";
[@bs.send] external logout: app => unit;
[@bs.send]
external loginPopup: (app, scopes) => Js.Promise.t(string) = "loginPopup";
[@bs.send]
external acquireTokenSilent: (app, scopes) => Js.Promise.t(string) =
  "acquireTokenSilent";
[@bs.send]
external acquireTokenPopup: (app, scopes) => Js.Promise.t(string) =
  "acquireTokenPopup";

[@bs.module "msal"] [@bs.new]
external make_:
  (string, string, option(tokenReceivedCallback), option(config)) => app =
  "UserAgentApplication";

let make =
    (
      ~clientId: string,
      ~authority: string,
      ~tokenReceivedCallback: option(tokenReceivedCallback)=?,
      ~config: option(config)=?,
      (),
    ) =>
  make_(clientId, authority, tokenReceivedCallback, config);