type fallbackComponentProps('s, 'e) = {
  componentStack: 's,
  error: 'e,
};

[@bs.obj]
external makeProps:
  (
    ~key: string=?,
    ~fallbackComponent: 'fallbackComponent=?,
    ~onError: 'onError=?,
    ~children: 'children,
    unit
  ) =>
  {
    .
    "FallbackComponent": 'fallbackComponent,
    "onError": 'onError,
    "children": 'children,
  };

[@bs.module "react-error-boundary"]
external make:
  {
    .
    "children": React.element,
    "FallbackComponent": fallbackComponentProps('s, 'e) => React.element,
    "onError": ('e, 's) => unit,
  } =>
  React.element =
  "default";