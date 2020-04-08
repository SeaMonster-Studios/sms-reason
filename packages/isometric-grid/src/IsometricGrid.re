open Belt;
let str = React.string;

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
      (~width as theWidth, ~row as theRow, ~col as theCol, ~show, ~children=?) => {
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
          padding(5->px),
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

let initItemsState = items => {
  items->Array.map(item => (item, true));
};

type status =
  | Measuring
  | Measured({
      containerWidth: int,
      itemWidth: int,
      itemHeight: int,
    });

[@react.component]
let make = (~columns=3, ~items as itemsProp, ~render, ~filter) => {
  let (ref, {ReactUseMeasure.width: containerWidth}) =
    ReactUseMeasure.(use(params(~polyfill, ())));

  let (status, setStatus) = React.useState(() => Measuring);
  let (items, setItems) = React.useState(() => itemsProp->initItemsState);

  React.useEffect2(
    () => {
      setItems(_ => itemsProp->initItemsState);
      None;
    },
    (itemsProp, setItems),
  );

  React.useEffect2(
    () => {
      setItems(items => items->Array.map(((x, _)) => (x, filter(x))));
      None;
    },
    (filter, setItems),
  );

  let (terpValues, setTerpValues) =
    HeightSpring.use(
      ~config=Spring.config(~mass=1.0, ~tension=110., ~friction=20.),
      0,
    );

  React.useEffect2(
    () => {
      setStatus(
        fun
        | Measured(_)
        | Measuring when containerWidth > 0 => {
            let width = containerWidth / columns;
            Measured({containerWidth, itemWidth: width, itemHeight: width});
          }
        | status => status,
      );
      None;
    },
    (containerWidth, setStatus),
  );

  let filteredItems =
    React.useMemo1(
      () => items->Array.keep(((_, show)) => show),
      [|items|],
    );

  React.useEffect3(
    () => {
      switch (status) {
      | Measured({itemHeight}) =>
        let filteredRows =
          (
            filteredItems->Array.length->Float.fromInt /. columns->Float.fromInt
          )
          ->ceil
          ->Float.toInt;
        let height = filteredRows * itemHeight;
        setTerpValues(height);
      | _ => ()
      };
      None;
    },
    (setTerpValues, filteredItems, status),
  );

  <div className=Css.([width(100.0->pct)]->style)>
    <div ref className=Css.([display(`flex)]->style)>
      {switch (status) {
       | Measuring => React.null
       | Measured({itemWidth}) =>
         <Spring.Div
           className=Css.(
             [
               label("grid-container"),
               position(`relative),
               flexGrow(1.0),
               outline(1->px, `solid, black),
               overflow(`hidden),
             ]
             ->style
           )
           style={ReactDOMRe.Style.make(
             ~height=terpValues->HeightSpring.interpolate,
             (),
           )}>
           {items
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
                  key={i->string_of_int}
                  width=itemWidth
                  row=rowNum
                  col=colNum
                  show>
                  {render(n)}
                </Box>;
              })
            ->React.array}
         </Spring.Div>
       }}
    </div>
  </div>;
};