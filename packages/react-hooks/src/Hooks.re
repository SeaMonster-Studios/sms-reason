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