[@react.component] [@bs.module "./ReactErrorBoundary.js"]
external make:
  (
    ~children: React.element,
    ~onError: ('e, 'i) => unit=?,
    ~renderFallback: ('e, 'i) => React.element=?
  ) =>
  React.element =
  "ReactErrorBoundary";