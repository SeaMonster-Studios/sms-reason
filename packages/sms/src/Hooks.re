open Belt;

[@bs.module "react"]
external useMemo8:
  ([@bs.uncurry] (unit => 'any), ('a, 'b, 'c, 'd, 'e, 'f, 'g, 'h)) => 'any =
  "useMemo";

[@bs.module "react"]
external useCallback4:
  ([@bs.uncurry] ('input => 'output), ('a, 'b, 'c, 'd)) =>
  React.callback('input, 'output) =
  "useCallback";

[@bs.module "react"]
external useCallback5:
  ([@bs.uncurry] ('input => 'output), ('a, 'b, 'c, 'd, 'e)) =>
  React.callback('input, 'output) =
  "useCallback";

[@bs.module "react"]
external useCallback6:
  ([@bs.uncurry] ('input => 'output), ('a, 'b, 'c, 'd, 'e, 'f)) =>
  React.callback('input, 'output) =
  "useCallback";

[@bs.module "react"]
external useCallback7:
  ([@bs.uncurry] ('input => 'output), ('a, 'b, 'c, 'd, 'e, 'f, 'g)) =>
  React.callback('input, 'output) =
  "useCallback";

[@bs.module "react"]
external useEffect6:
  ([@bs.uncurry] (unit => option(unit => unit)), ('a, 'b, 'c, 'd, 'e, 'f)) =>
  unit =
  "useEffect";

[@bs.module "react"]
external useEffect7:
  (
    [@bs.uncurry] (unit => option(unit => unit)),
    ('a, 'b, 'c, 'd, 'e, 'f, 'g)
  ) =>
  unit =
  "useEffect";

[@bs.module "react"]
external useEffect8:
  (
    [@bs.uncurry] (unit => option(unit => unit)),
    ('a, 'b, 'c, 'd, 'e, 'f, 'g, 'h)
  ) =>
  unit =
  "useEffect";

[@bs.module "react"]
external useEffect15:
  (
    [@bs.uncurry] (unit => option(unit => unit)),
    ('a, 'b, 'c, 'd, 'e, 'f, 'g, 'h, 'i, 'j, 'k, 'l, 'm, 'n)
  ) =>
  unit =
  "useEffect";

let useAsyncStateEffect4 = (callback, dependencies) => {
  React.useEffect4(
    () => {
      let didCancel = ref(false);

      switch (callback(), didCancel^ == false) {
      | (Some(returnedValue), true) => returnedValue()
      | (Some(_), false)
      | (None, true)
      | (None, false) => ()
      };

      Some(() => {didCancel := true});
    },
    dependencies,
  );
};

let useHookWithDecoderToJs = (hookBase, decoder, ()) => {
  let item = hookBase();

  React.useMemo2(() => item->decoder, (item, decoder));
};

let useHookOptionalValueWithDecoderToJs = (hookBase, decoder, ()) => {
  let item = hookBase();

  React.useMemo2(
    () =>
      switch (item) {
      | None => None
      | Some(item) => Some(item->decoder)
      },
    (item, decoder),
  );
};

let usePrevious = (value: 'a) => {
  let reference = React.useRef(value);
  React.useEffect1(
    () => {
      React.Ref.setCurrent(reference, value);
      Some(() => ());
    },
    [|value|],
  );
  React.Ref.current(reference);
};

let useInterval = (callback: unit => unit, delay) => {
  let savedCallback = React.useRef(() => ());

  React.useEffect1(
    () => {
      React.Ref.setCurrent(savedCallback, callback);
      Some(() => ());
    },
    [|callback|],
  );

  React.useEffect1(
    () => {
      let tick = () => {
        let current = React.Ref.current(savedCallback);
        current();
      };
      switch (delay) {
      | None => Some(() => ())
      | Some(delay) =>
        let id = Js.Global.setInterval(tick, delay);
        Some(() => id->Js.Global.clearInterval);
      };
    },
    [|delay|],
  );
};

/** This is useful for testing error boundaries. Be sure to use it within a child component of an error boundary */
let useRenderError = () => {
  let (error, setError) = React.useState(() => false);

  React.useCallback2(
    () => {
      let makeError: bool => unit = [%raw
        {|
      function () {
        // eslint-disable-next-line
        error.nothingHere()
      }
    |}
      ];

      makeError(error);
    },
    (error, setError),
  );
};

external nullElement: Js.Nullable.t('a) => Dom.element = "%identity";

let useIdRef = id => {
  let el =
    Document.getElementById(id)
    ->Option.getWithDefault(Js.Nullable.null->nullElement);
  let ref_ = React.useRef(el);

  React.useEffect2(
    () => {
      React.Ref.(ref_->setCurrent(el));
      None;
    },
    (ref_, el),
  );

  ref_;
};