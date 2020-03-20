type listItemRenderer('a) =
  {
    .
    "data": 'a,
    "index": int,
    "style": ReactDOMRe.Style.t,
  } =>
  React.element;

type onItemsRenderedValue = {
  overscanStartIndex: int,
  overscanStopIndex: int,
  visibleStartIndex: int,
  visibleStopIndex: int,
};

type onScrollValue = {
  scrollTop: int,
  scrollDirection: string,
  scrollOffset: int,
  scrollUpdateWasRequested: bool,
};

module List = {
  [@bs.module "react-window"] [@react.component]
  external make:
    (
      ~height: int,
      ~itemCount: int,
      ~itemSize: int,
      ~width: string,
      ~layout: string=?,
      ~itemData: 'b=?,
      ~outerRef: ReactDOMRe.Ref.t=?,
      ~innerRef: ReactDOMRe.Ref.t=?,
      ~overscanCount: int=?,
      ~useIsScrolling: bool=?,
      ~onItemsRendered: onItemsRenderedValue => unit=?,
      ~onScroll: onScrollValue => unit=?,
      ~className: string=?,
      ~children: listItemRenderer('a)
    ) =>
    React.element =
    "FixedSizeList";
};

type cellItemRenderer('a) =
  {
    .
    "data": 'a,
    "rowIndex": int,
    "columnIndex": int,
    "style": ReactDOMRe.Style.t,
  } =>
  React.element;

module Grid = {
  [@bs.module "react-window"] [@react.component]
  external make:
    (
      ~columnCount: int,
      ~columnWidth: int,
      ~height: int,
      ~rowCount: int,
      ~itemSize: int,
      ~width: int,
      ~rowHeight: int,
      ~itemData: 'b=?,
      ~ref: ReactDOMRe.Ref.t=?,
      ~overscanCount: int=?,
      ~useIsScrolling: bool=?,
      ~onItemsRendered: onItemsRenderedValue => unit=?,
      ~onScroll: onScrollValue => unit=?,
      ~className: string=?,
      ~children: cellItemRenderer('a)
    ) =>
    React.element =
    "FixedSizeGrid";
};