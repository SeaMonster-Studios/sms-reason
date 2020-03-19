[@bs.val] [@bs.return nullable] [@bs.scope "document"]
external getElementById: string => option(Dom.element) = "getElementById";

[@bs.val]
external appendChild: Dom.element => Dom.element = "document.body.appendChild";

[@bs.val]
external removeChild: Dom.element => Dom.element = "document.body.removeChild";

[@bs.val] [@bs.scope "document"]
external createElement: string => Dom.element = "createElement";

[@bs.val] [@bs.scope "document"]
external addEventListener: (string, 'a => unit) => unit = "addEventListener";

[@bs.val] [@bs.scope "document"]
external removeEventListener: (string, 'a => unit) => unit =
  "removeEventListener";

type classList = {
  .
  [@bs.meth] "add": string => unit,
  [@bs.meth] "remove": string => unit,
};

type element = {
  .
  [@bs.meth] "querySelector": string => element,
  [@bs.meth] "appendChild": element => element,
  "classList": classList,
};

external domElToDomEl_: Dom.element => element = "%identity";
external _domElToDomEl: element => Dom.element = "%identity";