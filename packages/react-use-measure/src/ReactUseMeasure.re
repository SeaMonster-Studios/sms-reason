type resizeObserverPolyfill;

[@bs.module "@juggle/resize-observer"]
external polyfill: resizeObserverPolyfill = "ResizeObserver";

type debounce = {
  scroll: int,
  resize: float,
};

[@bs.deriving abstract]
type params = {
  [@bs.optional]
  polyfill: resizeObserverPolyfill,
  [@bs.optional]
  debounce,
  [@bs.optional]
  scroll: bool,
};

[@bs.module "react"]
external useEffect2:
  ([@bs.uncurry] (unit => option(unit => unit)), ('a, 'b)) => unit =
  "useEffect";

type bounds = {
  width: int,
  height: int,
  left: float,
  top: float,
  bottom: float,
  right: float,
  x: float,
  y: float,
};

[@bs.module]
external useHook: params => (ReactDOMRe.domRef, bounds) = "react-use-measure";