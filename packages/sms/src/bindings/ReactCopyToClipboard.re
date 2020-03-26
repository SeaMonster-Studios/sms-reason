 [@react.component] [@bs.module "react-copy-to-clipboard"]
  external make:
    (~text: string, ~onCopy: unit => unit, ~children: React.element) =>
    React.element =
    "default";