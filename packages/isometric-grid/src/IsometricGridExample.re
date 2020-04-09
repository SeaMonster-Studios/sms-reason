open Belt;

let str = React.string;

module GridItem = {
  let pct100 = Css.(100.->pct);
  let str = React.string;

  [@react.component]
  let make = (~name) => {
    let imageRandomizer = React.useMemo0(() => Random.int(999999));
    <div
      className=Css.(
        [
          width(100.0->pct),
          height(100.0->pct),
          position(`relative),
          cursor(`pointer),
          overflow(`hidden),
          selector(
            "&:hover .item__bg-image",
            [Css.transform(scale(1.1, 1.1))],
          ),
        ]
        ->style
      )>
      <div
        className=Css.(
          [
            zIndex(-1),
            position(`absolute),
            width(100.0->pct),
            height(100.0->pct),
            backgroundColor(`hex("99cc66")),
            backgroundPosition(`center),
            backgroundSize(`cover),
            transition(~duration=500, ~timingFunction=`easeOut, "transform"),
            backgroundImage(
              `url(
                {j|"https://picsum.photos/300?imageCacheBuster=$imageRandomizer"|j},
              ),
            ),
          ]
          ->style
          ++ " item__bg-image"
        )
      />
      <div className=Css.([flexGrow(1.0)]->style)> name->str </div>
    </div>;
  };
};

let filter1 = n => n mod 2 == 0;
let filter2 = n => n < 5;

let render = n => <GridItem name={n->string_of_int} />;

[@react.component]
let make = () => {
  let items = React.useMemo0(() => Array.makeBy(10, n => n));

  let (filterBy1, setFilterBy1) = React.useState(() => false);
  let (filterBy2, setFilterBy2) = React.useState(() => false);

  let filter =
    React.useCallback2(
      item =>
        true
        && {
          filterBy1 ? filter1(item) : true;
        }
        && (filterBy2 ? filter2(item) : true),
      (filterBy1, filterBy2),
    );

  <div className="example">
    <div className="toolbar">
      <button onClick={_ => setFilterBy1(b => !b)}> "filter"->str </button>
      <button onClick={_ => setFilterBy2(b => !b)}> "filter2"->str </button>
    </div>
    <IsometricGrid columns=3 items render filter padding=10/>
  </div>;
};