module Location = {
  [@bs.val] [@bs.scope ("window", "location")] external href: string = "href";
  [@bs.val] [@bs.scope ("window", "location")] external hash: string = "hash";
  [@bs.val] [@bs.scope ("window", "location")]
  external search: string = "search";
};