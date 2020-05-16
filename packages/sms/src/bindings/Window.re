module Location = {
  [@bs.val] [@bs.scope ("window", "location")] external href: string = "href";
  [@bs.val] [@bs.scope ("window", "location")] external hash: string = "hash";
  [@bs.val] [@bs.scope ("window", "location")]
  external search: string = "search";
  [@bs.val] [@bs.scope ("window", "location")] external host: string = "host";
  [@bs.val] [@bs.scope ("window", "location")]
  external hostname: string = "hostname";
};