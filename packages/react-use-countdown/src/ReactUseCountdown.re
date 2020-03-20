/** matches css animation play states */
type playState = [ | `running | `paused];

type value = {
  time: int,
  play: unit => unit,
  pause: unit => unit,
  restart: unit => unit,
  playState,
};

type state = {
  time: int,
  playState,
};

type action =
  | Play
  | Pause
  | Restart(int)
  | Decrement(unit => unit);

let frequency = 100;

let reducer = (state: state, action) => {
  switch (action) {
  | Play => {...state, playState: `running}
  | Pause => {...state, playState: `paused}
  | Restart(time) => {playState: `running, time}
  | Decrement(onEnd) =>
    let nextTime = state.time - frequency;

    if (nextTime <= 0) {
      onEnd();
      {time: 0, playState: `paused};
    } else {
      {...state, time: nextTime};
    };
  };
};

let createInterval = (fn, frequency) =>
  Js.Global.setInterval(() => fn(), frequency);

let useHook = (duration, onEnd) => {
  let timerCallback = React.useRef(() => ());

  let (state, dispatch) =
    React.useReducer(reducer, {playState: `paused, time: duration});

  React.useEffect2(
    () => {
      React.Ref.setCurrent(timerCallback, () => dispatch(Decrement(onEnd)));
      Some(() => ());
    },
    (onEnd, dispatch),
  );

  React.useEffect2(
    () => {
      let tick = () => {
        let current = React.Ref.current(timerCallback);
        current();
      };
      switch (state.playState, state.time == 0) {
      | (`paused, true)
      | (`paused, false)
      | (`running, true) => Some(() => ())
      | (`running, false) =>
        let id = Js.Global.setInterval(tick, frequency);
        Some(() => id->Js.Global.clearInterval);
      };
    },
    (timerCallback, state),
  );

  let play =
    React.useCallback2(
      () =>
        if (state.time != 0) {
          dispatch(Play);
        },
      (state.time, dispatch),
    );

  let pause = React.useCallback1(() => dispatch(Pause), [|dispatch|]);

  let restart =
    React.useCallback2(
      () => dispatch(Restart(duration)),
      (duration, dispatch),
    );

  React.useMemo5(
    () =>
      {time: state.time, play, restart, pause, playState: state.playState},
    (state.time, play, restart, pause, state.playState),
  );
};