// https://github.com/tannerlinsley/react-query-devtools
[@bs.deriving abstract]
type props = {
  [@bs.optional]
  className: string,
  [@bs.optional]
  style: ReactDOMRe.Style.t,
};

[@bs.module "react-query-devtools"] [@react.component]
external make:
  (
    ~initialIsOpen: bool=?,
    ~className: string=?,
    ~panelProps: props=?,
    ~closeButtonProps: props=?,
    ~toggleButtonProps: props=?
  ) =>
  React.element =
  "ReactQueryDevtools";