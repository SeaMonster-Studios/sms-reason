type itemRenderer('a) =
  {
    .
    "data": 'a,
    "index": int,
    "style": ReactDOMRe.Style.t,
  } =>
  React.element;

module FixedSizeList = {
  [@bs.module "react-window"] [@react.component]
  external make:
    (
      ~height: int,
      ~itemCount: int,
      ~itemSize: int,
      ~width: string,
      ~layout: string=?,
      ~itemData: 'b=?,
      ~ref: ReactDOMRe.Ref.t=?,
      ~overscanCount: int=?,
      ~useIsScrolling: bool=?,
      ~onItemsRendered: unit => unit=?,
      ~children: itemRenderer('a)
    ) =>
    React.element =
    "FixedSizeList";
};