type value = {. [@bs.meth] "load": unit => Js.Promise.t(unit)};

[@bs.new] [@bs.module]
external make: (string, Js.Dict.t('a)) => value = "fontfaceobserver";