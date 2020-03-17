type align = [ | `right | `left];

module Style = {
  open Css;

  let root =
    style([
      zIndex(50),
      display(`inlineFlex),
      flexDirection(`column),
      position(`relative),
    ]);

  let content = align =>
    merge([
      style(
        switch (align) {
        | `left => [left(0->px), selector(".triangle", [left(10->px)])]
        | `right => [right(0->px), selector(".triangle", [right(10->px)])]
        },
      ),
      style([
        position(`absolute),
        zIndex(100),
        top(`calc((`add, 100.->pct, 15->px))),
        selector(
          ".triangle",
          [
            width(0->px),
            height(0->px),
            borderLeft(10->px, `solid, `transparent),
            borderRight(10->px, `solid, `transparent),
            borderBottom(13->px, `solid, Colors.white),
            display(`block),
            position(`absolute),
            top(0->px),
            transform(translateY((-100.)->pct)),
          ],
        ),
        selector(
          "> div",
          [
            margin(0->px),
            background(Colors.white),
            boxShadow(
              Shadow.box(
                ~x=2->px,
                ~y=2->px,
                ~blur=16->px,
                rgba(0, 0, 0, 0.15),
              ),
            ),
            minWidth(200->px),
          ],
        ),
      ]),
    ]);
};

type renderProps = {
  toggle: unit => unit,
  close: unit => unit,
  open_: unit => unit,
};

let elContainsTarget: (Dom.element, ReactEvent.Mouse.t) => bool = [%raw
  {|
	function scroll(element, event) {
		return element.contains(event.target)
	}
|}
];

[@react.component]
let make =
    (
      ~children: renderProps => React.element,
      ~align,
      ~renderToggler: renderProps => React.element,
    ) => {
  let (isActive, setIsActive) = React.useState(() => false);
  let rootRef = React.useRef(Js.Nullable.null);

  let outsideClickListener =
    React.useCallback1(
      (event: ReactEvent.Mouse.t) => {
        rootRef
        ->React.Ref.current
        ->Js.Nullable.toOption
        ->(
            fun
            | Some((el: Dom.element)) when elContainsTarget(el, event) => {
                setIsActive(_ => true);
              }
            | _ => ()
          )
      },
      [|setIsActive|],
    );

  React.useEffect1(
    () => {
      Document.addEventListener("click", outsideClickListener);

      Some(
        () => Document.removeEventListener("click", outsideClickListener),
      );
    },
    [|outsideClickListener|],
  );

  let renderProps =
    React.useMemo1(
      () =>
        {
          open_: () => setIsActive(_ => true),
          close: () => setIsActive(_ => false),
          toggle: () => setIsActive(isActive => !isActive),
        },
      [|setIsActive|],
    );

  <div ref={rootRef->ReactDOMRe.Ref.domRef}>
    <div className=Style.root>
      {renderToggler(renderProps)}
      {isActive
         ? <div className={Style.content(align)}>
             <span className="triangle" />
             <div> {children(renderProps)} </div>
           </div>
         : React.null}
    </div>
  </div>;
};