[@bs.val] [@bs.scope "window"] external top: int = "top";
[@bs.val] [@bs.scope "window"] external self: int = "self";

module Location = {
  [@bs.val] [@bs.scope ("window", "location")] external href: string = "href";
  [@bs.val] [@bs.scope ("window", "location")] external hash: string = "hash";
  [@bs.val] [@bs.scope ("window", "location")]
  external search: string = "search";
  [@bs.val] [@bs.scope ("window", "location")] external host: string = "host";
  [@bs.val] [@bs.scope ("window", "location")]
  external hostname: string = "hostname";
  [@bs.val] [@bs.scope ("window", "location")]
  external assignUrl: string => unit = "assign";
};