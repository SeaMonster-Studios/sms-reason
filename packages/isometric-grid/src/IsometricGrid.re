open Belt;
let str = React.string;

type paged = {
  itemsPerPage: int,
  openPages: int,
};

type paging =
  | Single
  | Paged(paged);

module Box = {
  module TransformSpring = {
    type t = (int, int);
    include Spring.MakeSpring({
      type nonrec t = t;
      type interpolate = (int, int) => string;
    });

    type set = (~config: Spring.config=?, t) => unit;

    let interpolate =
      interpolate(_, (x, y) => {j|translateX($(x)px) translateY($(y)px)|j});
  };

  module OpacitySpring = {
    type t = int;
    include Spring.MakeSpring({
      type nonrec t = t;
      type interpolate = int => string;
    });

    type set = (~config: Spring.config=?, t) => unit;

    let interpolate = interpolate(_, opacity => {j|$opacity|j});
  };

  let springConfig = Spring.config(~mass=1.0, ~tension=110., ~friction=20.);

  let affectTransform =
      (
        ~show,
        ~left,
        ~top,
        ~width,
        ~setTransformSpring: TransformSpring.set,
        (),
      ) => {
    setTransformSpring((left, show ? top : top - width));
    None;
  };

  let affectOpacity = (~show, ~setOpacitySpring: OpacitySpring.set, ()) => {
    setOpacitySpring(show ? 1 : 0);
    None;
  };

  [@react.component]
  let make =
      (
        ~width as theWidth,
        ~row as theRow,
        ~col as theCol,
        ~show,
        ~padding as thePadding,
        ~children=?,
      ) => {
    let theTop = React.useMemo2(() => theRow * theWidth, (theRow, theWidth));
    let theLeft =
      React.useMemo2(() => theCol * theWidth, (theCol, theWidth));

    let (transformSpring, setTransformSpring) =
      TransformSpring.use(~config=springConfig, (0, theTop));

    let (opacitySpring, setOpacitySpring) =
      OpacitySpring.use(~config=springConfig, 0);

    React.useEffect5(
      affectTransform(
        ~show,
        ~left=theLeft,
        ~top=theTop,
        ~width=theWidth,
        ~setTransformSpring,
      ),
      (show, theLeft, theTop, theWidth, setTransformSpring),
    );

    React.useEffect2(
      affectOpacity(~show, ~setOpacitySpring),
      (show, setOpacitySpring),
    );

    <Spring.Div
      style={ReactDOMRe.Style.make(
        ~transform=transformSpring->TransformSpring.interpolate,
        ~opacity=opacitySpring->OpacitySpring.interpolate,
        (),
      )}
      className=Css.(
        [
          display(`flex),
          width(theWidth->px),
          height(theWidth->px),
          position(`absolute),
          padding(thePadding->px),
          pointerEvents(show ? `auto : none),
          zIndex(show ? 2 : 1),
        ]
        ->style
      )
      ?children
    />;
  };
};

module HeightSpring = {
  include Spring.MakeSpring({
    type t = int;
    type interpolate = t => string;
  });

  let interpolate = interpolate(_, height => {height->string_of_int ++ "px"});
};

module RunningView = {
  [@react.component]
  let make =
      (
        ~items,
        ~itemWidth,
        ~itemHeight,
        ~columns,
        ~render,
        ~padding,
        ~paging as pagingProp,
        ~getKey,
        ~buttonClass,
      ) => {
    let (paging, setPaging) = React.useState(() => pagingProp);
    React.useEffect2(
      () => {
        setPaging(_ => pagingProp);
        None;
      },
      (pagingProp, setPaging),
    );

    let filteredItems =
      React.useMemo1(
        () => items->Array.keep(((_, show)) => show),
        [|items|],
      );

    let (heightSpring, setHeightSpring) =
      HeightSpring.use(
        ~config=Spring.config(~mass=1.0, ~tension=110., ~friction=20.),
        0,
      );

    let maxDisplayedItems =
      React.useMemo1(
        () =>
          switch (paging) {
          | Single => items->Array.length
          | Paged({itemsPerPage, openPages}) => itemsPerPage * openPages
          },
        [|paging|],
      );

    React.useEffect2(
      () => {
        let filteredRows = {
          let numfilteredItems = filteredItems->Array.length;
          let numDisplayedItems =
            numfilteredItems <= maxDisplayedItems
              ? numfilteredItems : maxDisplayedItems;
          (numDisplayedItems->Float.fromInt /. columns->Float.fromInt)
          ->ceil
          ->Float.toInt;
        };
        let height = filteredRows * itemHeight;
        setHeightSpring(height);

        None;
      },
      (setHeightSpring, filteredItems),
    );

    <>
      <Spring.Div
        className=Css.(
          [label("grid-container"), position(`relative), flexGrow(1.0)]
          ->style
        )
        style={ReactDOMRe.Style.make(
          ~height=heightSpring->HeightSpring.interpolate,
          (),
        )}>
        {let (shownItems, hiddenItems) =
           items->Array.partition(((_, show)) => show);
         let sortedItems = Array.concat(shownItems, hiddenItems);
         sortedItems
         ->Array.slice(~offset=0, ~len=maxDisplayedItems)
         ->Array.mapWithIndex((i, (n, show) as tuple) => {
             let (rowNum, colNum) =
               switch (filteredItems->Array.getIndexBy(x => x == tuple)) {
               | None => (i / columns, i mod columns)
               | Some(iFiltered) => (
                   iFiltered / columns,
                   iFiltered mod columns,
                 )
               };

             <Box
               key={n->getKey}
               width=itemWidth
               row=rowNum
               col=colNum
               show
               padding>
               {render(n)}
             </Box>;
           })
         ->React.array}
      </Spring.Div>
      {maxDisplayedItems < filteredItems->Array.length
         ? <button
             className=buttonClass
             onClick={_ =>
               setPaging(
                 fun
                 | Single => Single
                 | Paged({openPages, itemsPerPage}) =>
                   Paged({itemsPerPage, openPages: openPages + 1}),
               )
             }>
             "Load more"->str
           </button>
         : React.null}
    </>;
  };
};

let initItemsState = items => {
  items->Array.map(item => (item, true));
};

let applyFilter = (items, filter) =>
  items->Array.map(((x, _)) => (x, filter(x)));

type show = bool;
type filter('item) = 'item => show;

type runConfig('item) = {
  items: array('item),
  containerWidth: int,
  columns: int,
  filter: filter('item),
};

type action('item) =
  | Run(runConfig('item))
  | SetItems(array('item))
  | SetFilter(filter('item))
  | ChangeLayout({
      containerWidth: int,
      columns: int,
    });

type status('item) =
  | Loading
  | Running({
      items: array(('item, show)),
      itemWidth: int,
      itemHeight: int,
      columns: int,
    });

let calcContainerHeight = (items, columns, itemHeight) => {
  let filteredItems = items->Array.keep(((_, show)) => show);
  let numRows =
    {
      filteredItems->Array.length->Float.fromInt /. columns->Float.fromInt;
    }
    ->ceil
    ->Float.toInt;
  numRows * itemHeight;
};

let reducer = (status, action) => {
  switch (status, action) {
  | (Loading, Run(runConfig)) =>
    let {items, filter, containerWidth, columns} = runConfig;
    let width = containerWidth / columns;
    let items = items->initItemsState->applyFilter(filter);
    Running({items, itemWidth: width, itemHeight: width, columns});

  | (Running(state), SetItems(items)) =>
    Running({...state, items: items->initItemsState})

  | (Running(state), SetFilter(filter)) =>
    Running({...state, items: state.items->applyFilter(filter)})

  | (Running(state), ChangeLayout({containerWidth, columns})) =>
    let width = containerWidth / state.columns;
    Running({...state, columns, itemWidth: width, itemHeight: width});

  | _ => status
  };
};

[@react.component]
let make =
    (
      ~columns=3,
      ~items as itemsProp,
      ~render,
      ~filter,
      ~padding=0,
      ~getKey,
      ~paging=Single,
      ~buttonClass="isometric-grid__load-more-button"
    ) => {
  let (ref, {ReactUseMeasure.width: containerWidth}) =
    ReactUseMeasure.(useHook(params(~polyfill, ())));

  let (status, dispatch) = React.useReducer(reducer, Loading);

  React.useEffect2(
    () => {
      dispatch(SetItems(itemsProp));
      None;
    },
    (itemsProp, dispatch),
  );

  React.useEffect2(
    () => {
      dispatch(SetFilter(filter));
      None;
    },
    (filter, dispatch),
  );

  React.useEffect3(
    () => {
      containerWidth == 0
        ? ()
        : (
          switch (status) {
          | Loading =>
            dispatch(
              Run({items: itemsProp, containerWidth, filter, columns}),
            )

          | Running(_) => dispatch(ChangeLayout({containerWidth, columns}))
          }
        );
      None;
    },
    (containerWidth, columns, dispatch),
  );

  <div className=Css.([width(100.0->pct)]->style)>
    <div ref className=Css.([display(`flex), flexDirection(`column)]->style)>
      {switch (status) {
       | Loading => React.null
       | Running({items, itemHeight, itemWidth}) =>
         <RunningView
           items
           itemHeight
           itemWidth
           columns
           render
           padding
           paging
           getKey
           buttonClass
         />
       }}
    </div>
  </div>;
};