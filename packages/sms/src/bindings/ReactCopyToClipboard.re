 [@react.component] [@bs.module]
  external make:
    (~text: string, ~onCopy: unit => unit, ~children: React.element) =>
    React.element =
    "react-copy-to-clipboard";