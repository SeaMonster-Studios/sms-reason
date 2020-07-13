module Sms = SeamonsterStudiosReason;

type shownNoticesType =
  | Active
  | All;

type toggleNoticeTypeButtonPalette = {
  text: Css.Types.Color.t,
  background: Css.Types.Color.t,
};

type palette = {
  accent: Css.Types.Color.t,
  info: Css.Types.Color.t,
  success: Css.Types.Color.t,
  warning: Css.Types.Color.t,
  error: Css.Types.Color.t,
  toggleNoticeTypeButton: toggleNoticeTypeButtonPalette,
};

let defaultPalette =
  Css.{
    accent: rgba(0, 0, 0, 0.75),
    info: rgb(132, 210, 250),
    success: rgb(157, 250, 176),
    warning: rgb(250, 219, 120),
    error: rgb(250, 172, 145),
    toggleNoticeTypeButton: {
      background: rgb(132, 213, 250),
      text: rgba(0, 0, 0, 0.75),
    },
  };

module Style = {
  open Css;

  module Common = {
    let boxShadow =
      boxShadow(
        Shadow.box(~x=px(2), ~y=px(2), ~blur=px(10), rgba(0, 0, 0, 0.15)),
      );
    let white = rgb(255, 255, 255);
  };

  let board = (p: palette) =>
    merge([
      "notice--board",
      style([
        position(`fixed),
        bottom(0->px),
        right(0->px),
        zIndex(99999999),
        display(`flex),
        flexDirection(`column),
        padding(20->px),
        selector(
          ".notice--close-icon",
          [
            padding(10->px),
            margin((-10)->px),
            cursor(`pointer),
            selector("path", [SVG.fill(p.accent)]),
          ],
        ),
        selector(
          ".notices--notice",
          [
            borderLeftColor(p.accent),
            color(p.accent),
            selector(
              ".notice--life-bar > div",
              [backgroundColor(p.accent)],
            ),
          ],
        ),
        selector(".notice--info", [backgroundColor(p.info)]),
        selector(".notice--success", [backgroundColor(p.success)]),
        selector(".notice--warning", [backgroundColor(p.warning)]),
        selector(".notice--error", [backgroundColor(p.error)]),
        selector(
          ".notices--type-toggle",
          [
            backgroundColor(p.toggleNoticeTypeButton.background),
            selector("button", [color(p.toggleNoticeTypeButton.text)]),
          ],
        ),
        selector(".notice--loader > div > div", [background(p.accent)]),
      ]),
    ]);

  let noticesContainer =
    merge([
      "notices--container",
      style([
        overflowY(`auto),
        display(`grid),
        maxHeight(`calc((`sub, 100.->vh, 20->px))),
        selector("> div", [overflow(`hidden)]),
      ]),
    ]);

  let toggleAllNotices = visible =>
    merge([
      "notices--type-toggle",
      style([
        opacity(visible ? 1. : 0.),
        Common.boxShadow,
        borderRadius(100.->pct),
        position(`absolute),
        bottom(0->px),
        right(0->px),
        width(visible ? 50->px : 0->px),
        height(visible ? 50->px : 0->px),
        overflow(`hidden),
        display(`flex),
        alignItems(`center),
        justifyContent(`center),
        transition(~duration=300, ~timingFunction=`ease, "all"),
        transforms([translateX(25->px), translateY(25->px)]),
        selector(
          "button",
          [
            opacity(0.),
            fontWeight(`bold),
            cursor(`pointer),
            background(`none),
            borderStyle(`none),
            textTransform(`uppercase),
            selector("&:focus", [outlineStyle(`none)]),
          ],
        ),
        selector(
          "&:hover",
          [
            borderRadius(5->px),
            width(100->px),
            transforms([translateX(5->px), translateY(5->px)]),
            selector("button", [opacity(1.)]),
          ],
        ),
      ]),
    ]);

  module Notice = {
    let base =
      merge([
        "notices--notice",
        style([
          width(275->px),
          backgroundColor(Common.white),
          Common.boxShadow,
          borderLeftWidth(4->px),
          borderLeftStyle(`solid),
          display(`flex),
          flexDirection(`column),
          marginTop(10->px),
          borderRadius(3->px),
        ]),
      ]);

    let info = merge([base, "notice--info"]);

    let success = merge([base, "notice--success"]);

    let warning = merge([base, "notice--warning"]);

    let error = merge([base, "notice--error"]);

    let lifeBarAnimation =
      keyframes([(0, [width(100.->pct)]), (100, [width(0.->pct)])]);

    let hideWithAllNoticeType = shownNoticeType =>
      style(
        switch (shownNoticeType) {
        | All => [opacity(0.), pointerEvents(`none)]
        | Active => []
        },
      );

    let life = (duration, playState, shownNoticeType) =>
      merge([
        hideWithAllNoticeType(shownNoticeType),
        "notice--life-bar",
        style([
          width(100.->pct),
          marginTop(10->px),
          selector(
            "> div",
            [
              height(2->px),
              opacity(0.35),
              animationName(lifeBarAnimation),
              animationDuration(duration),
              animationTimingFunction(`linear),
              animationFillMode(`forwards),
              animationPlayState(playState),
            ],
          ),
        ]),
      ]);

    let header =
      merge([
        "notice--header",
        style([
          padding3(~bottom=10->px, ~top=10->px, ~h=10->px),
          textTransform(`uppercase),
          fontWeight(`bold),
          fontSize(12->px),
          display(`flex),
          alignItems(`center),
          justifyContent(`spaceBetween),
        ]),
      ]);

    let content =
      merge([
        "notice--content",
        style([
          padding3(~top=0->px, ~bottom=10->px, ~h=10->px),
          fontSize(14->px),
          lineHeight(1.25->em),
        ]),
      ]);

    let loaderContent =
      merge([
        "notice--content",
        "notice--loader-content",
        style([
          selector(
            "> div:first-of-type",
            [
              display(`flex),
              alignItems(`center),
              justifyContent(`center),
              height(42->px),
              marginTop((-20)->px),
            ],
          ),
          selector(
            ".notice--life-bar",
            [
              margin(0->px),
              padding3(~bottom=10->px, ~h=20->px, ~top=0->px),
            ],
          ),
        ]),
      ]);

    let loader =
      merge([
        "notice--loader",
        style([
          transforms([scale(0.5, 0.5)]),
          selector(".inner-container", [margin(0->px)]),
        ]),
      ]);

    let removeButton = shownNoticeType =>
      merge([
        hideWithAllNoticeType(shownNoticeType),
        "notice--remove-button",
        style([
          position(`relative),
          zIndex(2),
          borderStyle(`none),
          background(`none),
          marginLeft(5->px),
          padding(0->px),
          selector(
            "svg",
            [width(10->px), height(`auto), marginTop((-5)->px)],
          ),
        ]),
      ]);
  };
};

open Belt;

let defaultLife = 10000;

let noticeGap = 10;

type type_ = [ | `success | `error | `info | `warning | `loading];

type singularNoticeRelationship = [ | `before | `after | `replace];

let handleNoticeContentRender =
    (
      globalContent,
      singularNoticeContent,
      relationship: singularNoticeRelationship,
    ) =>
  switch (relationship) {
  | `replace => singularNoticeContent
  | `before => <> singularNoticeContent globalContent </>
  | `after => <> globalContent singularNoticeContent </>
  };

type globalNoticeContent = {
  success: option((React.element, singularNoticeRelationship)),
  error: option((React.element, singularNoticeRelationship)),
  info: option((React.element, singularNoticeRelationship)),
  warning: option((React.element, singularNoticeRelationship)),
  loading: option((React.element, singularNoticeRelationship)),
};

type singleNoticeLife = option(int);
type newNotice = (type_, React.element, singleNoticeLife);
type updateNotice = (string, type_, React.element, singleNoticeLife);
type setNoticeHeight = (string, int);

type notice = {
  id: string,
  key: string,
  isActive: bool,
  element: React.element,
  life: int,
  type_,
};

type noticeSystemAction =
  | AddNotice(string, string, newNotice)
  | SetNoticeHeight(setNoticeHeight)
  | RemoveNotice(string)
  | SetNoticeLife(int)
  | SetShownNoticeType(shownNoticesType)
  | SetDispatch(noticeSystemAction => unit);

type noticeSystemState = {
  notices: array(notice),
  noticeHeights: Belt.Map.String.t(int),
  noticeLife: int, //milliseconds
  shownNoticesType,
  globalNoticeContent,
  dispatch: noticeSystemAction => unit,
};

let reducer = (state: noticeSystemState, action) =>
  switch (action) {
  | SetDispatch(dispatch) => {...state, dispatch}
  | SetShownNoticeType(shownNoticesType) => {...state, shownNoticesType}
  | AddNotice(id, key, (type_, element, singleNoticeLife)) =>
    switch (state.notices->Array.getBy(item => item.id == id)) {
    // Replace notice with new content, type_, etc.
    | Some(_notice) => {
        ...state,
        notices:
          state.notices
          ->Array.keep(notice => notice.id != id)
          ->Array.concat([|
              {
                type_,
                element,
                isActive: true,
                id,
                key,
                life:
                  switch (singleNoticeLife) {
                  | None => state.noticeLife
                  | Some(singleNoticeLife) => singleNoticeLife
                  },
              },
            |]),
        noticeHeights: Map.String.set(state.noticeHeights, id, 0),
      }
    | None =>
      // Create brand new notice
      let updated =
        state.notices
        ->Array.concat([|
            {
              type_,
              element,
              isActive: true,
              id,
              key,
              life:
                switch (singleNoticeLife) {
                | None => state.noticeLife
                | Some(singleNoticeLife) => singleNoticeLife
                },
            },
          |]);
      updated->Array.length > 20
        ? {
          ...state,
          notices: updated->Array.slice(~offset=1, ~len=20),
          noticeHeights: Map.String.set(state.noticeHeights, id, 0),
        }
        : {
          ...state,
          notices: updated,
          noticeHeights: Map.String.set(state.noticeHeights, id, 0),
        };
    }
  | SetNoticeHeight((id, height)) => {
      ...state,
      noticeHeights: Map.String.set(state.noticeHeights, id, height),
    }
  | RemoveNotice(key) => {
      ...state,
      notices:
        state.notices
        ->Array.map(notice =>
            notice.key == key ? {...notice, isActive: false} : notice
          ),
    }
  | SetNoticeLife(noticeLife) => {...state, noticeLife}
  };

let initialState = {
  notices: [||],
  noticeHeights: Map.String.empty,
  noticeLife: defaultLife,
  shownNoticesType: Active,
  dispatch: _ => (),
  globalNoticeContent: {
    success: None,
    error: None,
    info: None,
    warning: None,
    loading: None,
  },
};

let%private context = React.createContext(initialState);

module ContextProvider = {
  let make = React.Context.provider(context);
  let makeProps = (~value: noticeSystemState, ~children, ()) => {
    "value": value,
    "children": children,
  };
};

let use = () => React.useContext(context);

let%private getNoticeHeight = (noticeHeights, id) =>
  noticeHeights->Map.String.get(id);

let useNoticeHeight = id => {
  let noticeSystem = use();

  React.useMemo2(
    () => noticeSystem.noticeHeights->getNoticeHeight(id),
    (noticeSystem.noticeHeights, id),
  );
};

let useNotices = () => {
  let noticeSystem = use();

  React.useMemo1(
    () => noticeSystem.notices->Array.keep(notice => notice.isActive),
    [|noticeSystem.notices|],
  );
};

let useNotice = id => {
  let noticeSystem = use();

  React.useMemo2(
    () => noticeSystem.notices->Array.getBy(notice => notice.id == id),
    (noticeSystem.notices, id),
  );
};

let useSetShownNoticeType = () => {
  let noticeSystem = use();

  React.useCallback1(
    shownNoticesType => {
      noticeSystem.dispatch(SetShownNoticeType(shownNoticesType))
    },
    [|noticeSystem.dispatch|],
  );
};

let useRemoveNotice = () => {
  let noticeSystem = use();

  React.useCallback1(
    noticeId => {noticeSystem.dispatch(RemoveNotice(noticeId))},
    [|noticeSystem.dispatch|],
  );
};

module LoaderContent = {
  let className =
    Css.(
      style([
        display(`flex),
        justifyContent(`center),
        marginTop(35->px),
        marginBottom(35->px),
      ])
    );

  [@react.component]
  let make = () => {
    <div>
      <div className={Css.merge([className, Style.Notice.loader])}>
        <Sms.ReactSpinners.PropagateLoader size=20 loading=true />
      </div>
    </div>;
  };
};

module Notice = {
  module CloseIcon = {
    [@react.component]
    let make = (~className="") =>
      <svg
        className={Css.merge(["notice--close-icon", className])}
        width="35"
        height="35"
        viewBox="0 0 35 35"
        fill="none"
        xmlns="http://www.w3.org/2000/svg">
        <path
          d="M24.043 17.25L33.8086 27.0156C34.3945 27.6016 34.6875 28.3828 34.6875 29.1641C34.6875 30.043 34.3945 30.8242 33.8086 31.4102L31.6602 33.5586C30.9766 34.1445 30.1953 34.4375 29.4141 34.4375C28.5352 34.4375 27.8516 34.1445 27.2656 33.5586L17.5 23.793L7.73438 33.5586C7.14844 34.1445 6.36719 34.4375 5.58594 34.4375C4.70703 34.4375 3.92578 34.1445 3.33984 33.5586L1.19141 31.4102C0.605469 30.8242 0.3125 30.043 0.3125 29.1641C0.3125 28.3828 0.605469 27.6016 1.19141 27.0156L10.957 17.25L1.19141 7.48438C0.605469 6.89844 0.3125 6.21484 0.3125 5.33594C0.3125 4.55469 0.605469 3.77344 1.19141 3.08984L3.33984 0.941406C3.92578 0.355469 4.70703 0.0625 5.58594 0.0625C6.36719 0.0625 7.14844 0.355469 7.73438 0.941406L17.5 10.707L27.2656 0.941406C27.8516 0.355469 28.5352 0.0625 29.4141 0.0625C30.1953 0.0625 30.9766 0.355469 31.6602 0.941406L33.8086 3.08984C34.3945 3.77344 34.6875 4.55469 34.6875 5.33594C34.6875 6.21484 34.3945 6.89844 33.8086 7.48438L24.043 17.25Z"
          fill="#989898"
        />
      </svg>;
  };
  module Header = {
    [@react.component]
    let make = (~id, ~title) => {
      let noticeSystem = use();
      let notice = useNotice(id);
      let removeNotice = useRemoveNotice();

      <header className=Style.Notice.header>
        <div> title->React.string </div>
        {switch (noticeSystem.shownNoticesType) {
         | Active =>
           <button
             className={Style.Notice.removeButton(
               noticeSystem.shownNoticesType,
             )}
             type_="button"
             onClick={_ =>
               switch (notice) {
               | None => ()
               | Some(notice) => removeNotice(notice.key)
               }
             }>
             <CloseIcon />
           </button>
         | All => React.null
         }}
      </header>;
    };
  };

  /**id props on this component defaults to "", but the NoticeSystem always passes the id to the component. The user will not need to do this when instantiating this */
  [@react.component]
  let make = (~title=?, ~content, ~id="") => {
    let noticeSystem = use();
    let notice = useNotice(id);
    let removeNotice = useRemoveNotice();

    let life =
      React.useMemo2(
        () => {
          switch (noticeSystem.shownNoticesType, notice) {
          | (All, _) => 999999999
          | (Active, Some(notice)) => notice.life
          | (Active, _) => defaultLife
          }
        },
        (noticeSystem.shownNoticesType, notice),
      );

    let handleRemoveNotice =
      React.useCallback2(
        () =>
          switch (notice) {
          | None => ()
          | Some(notice) => removeNotice(notice.key)
          },
        (notice, removeNotice),
      );

    let countdown = ReactUseCountdown.use(life, handleRemoveNotice);

    React.useEffect1(
      () => {
        countdown.play();
        None;
      },
      [|countdown.play|],
    );

    let timerNode =
      switch (noticeSystem.shownNoticesType) {
      | Active =>
        <div
          className={Style.Notice.life(
            life,
            countdown.playState,
            noticeSystem.shownNoticesType,
          )}>
          <div />
        </div>
      | All => <div />
      };

    switch (notice) {
    | Some(notice) =>
      <div
        onMouseEnter={_ => {countdown.pause()}}
        onMouseLeave={_ => {countdown.play()}}
        className={
          switch (notice.type_) {
          | `info
          | `loading => Style.Notice.info
          | `success => Style.Notice.success
          | `error => Style.Notice.error
          | `warning => Style.Notice.warning
          }
        }>
        <Header
          id
          title={
            switch (title, notice.type_) {
            | (None, `loading) => "Loading"
            | (None, `info) => "Information"
            | (None, `success) => "Success"
            | (None, `warning) => "Warning"
            | (None, `error) => "Error"
            | (Some(title), _) => title
            }
          }
        />
        <div
          className={
            switch (notice.type_) {
            | `loading => Style.Notice.loaderContent
            | _ => Style.Notice.content
            }
          }>
          {switch (
             notice.type_,
             noticeSystem.globalNoticeContent.error,
             noticeSystem.globalNoticeContent.success,
             noticeSystem.globalNoticeContent.info,
             noticeSystem.globalNoticeContent.loading,
             noticeSystem.globalNoticeContent.warning,
           ) {
           | (`error, Some((custom, rel)), _, _, _, _) =>
             content->handleNoticeContentRender(custom, rel)
           | (`success, _, Some((custom, rel)), _, _, _) =>
             content->handleNoticeContentRender(custom, rel)
           | (`info, _, _, Some((custom, rel)), _, _) =>
             content->handleNoticeContentRender(custom, rel)
           | (`loading, _, _, _, Some((custom, rel)), _) =>
             content->handleNoticeContentRender(custom, rel)
           | (`warning, _, _, _, _, Some((custom, rel))) =>
             content->handleNoticeContentRender(custom, rel)
           | _ => content
           }}
          timerNode
        </div>
      </div>
    | None => React.null
    };
  };
};

let useAddNotice = () => {
  let noticeSystem = use();

  React.useMemo1(
    (
      (),
      ~id as id_=?,
      ~content=?,
      ~el=?,
      ~title=?,
      ~life: singleNoticeLife=?,
      type_,
    ) => {
      /** If an id is provided then this new notice will _replace_ the previous notice with that id. This is beneficial when using notices for acync states (Loading, Success, Error) */
      let key = Sms.Uuid.make();
      let element =
        switch (el, content) {
        | (Some(el), _) => el
        | (None, Some(content)) => <Notice content ?title />
        | (None, None) => <Notice content=React.null ?title />
        };

      let id =
        switch (id_) {
        | Some(id) => id
        | None => Sms.Uuid.make()
        };

      noticeSystem.dispatch(AddNotice(id, key, (type_, element, life)));
      id;
    },
    [|noticeSystem.dispatch|],
  );
};

type useNewValue = {
  notice: option(notice),
  add:
    (
      ~content: ReasonReact.reactElement=?,
      ~el: React.element=?,
      ~title: string=?,
      ~life: int=?,
      type_
    ) =>
    unit,
  remove: unit => unit,
  isActive: bool,
  id: string,
};

let useNew = () => {
  let noticeSystem = use();
  let noticeId = React.useMemo0(() => Sms.Uuid.make());
  let removeNotice = useRemoveNotice();
  let notice = useNotice(noticeId);

  let addNotice =
    React.useMemo1(
      ((), ~id, ~content=?, ~el=?, ~title=?, ~life: singleNoticeLife=?, type_) => {
        /** If an id is provided then this new notice will _replace_ the previous notice with that id. This is beneficial when using notices for acync states (Loading, Success, Error) */
        let key = Sms.Uuid.make();
        let element =
          switch (el, content) {
          | (Some(el), _) => el
          | (None, Some(content)) => <Notice content ?title />
          | (None, None) => <Notice content=React.null ?title />
          };

        noticeSystem.dispatch(AddNotice(id, key, (type_, element, life)));
      },
      [|noticeSystem.dispatch|],
    );

  React.useMemo4(
    () => {
      {
        notice,
        add: addNotice(~id=noticeId),
        remove: () => removeNotice(noticeId),
        id: noticeId,
        isActive:
          switch (notice) {
          | None => false
          | Some(notice) => notice.isActive
          },
      }
    },
    (notice, addNotice, removeNotice, noticeId),
  );
};

let%private useSetNoticeHeight = () => {
  let noticeSystem = use();

  React.useCallback1(
    setNoticeHeight => {
      noticeSystem.dispatch(SetNoticeHeight(setNoticeHeight))
    },
    [|noticeSystem.dispatch|],
  );
};

type animatedValue = {
  opacity: float,
  transform: string,
  height: int,
};

type animatedProps = notice => animatedValue;
external animatedValuesToStyle: animatedProps => ReactDOMRe.Style.t =
  "%identity";

module Component = {
  module TransitionHook = {
    include Spring.MakeTransition({
      type t = animatedProps;
      type item = notice;
    });
  };

  module Item = {
    [@react.component]
    let make = (~children, ~id) => {
      let (ref, bounds) = ReactUseMeasure.(useHook(params(~polyfill, ())));
      let setNoticeHeight = useSetNoticeHeight();
      let noticeHeight = useNoticeHeight(id);

      React.useEffect4(
        () => {
          switch (noticeHeight) {
          | None => ()
          | Some(currentHeight) =>
            /** We're not using this to control the animation value of the height (we control that with the useTransition)... As such we do not need to reset the height to 0... and then set it again if the notice becomes visible again. We really just want to set the height initially and then again if the content of the notice changes.  */
            (
              if (bounds.height != 0
                  && currentHeight != bounds.height
                  + noticeGap) {
                setNoticeHeight((id, bounds.height + noticeGap));
              }
            )
          };

          None;
        },
        (bounds.height, setNoticeHeight, id, noticeHeight),
      );

      <div ref className="notice-item"> children </div>;
    };
  };

  let springConfig =
    Spring.config(~tension=500., ~friction=70., ~mass=3., ~precision=0.1, ());

  type showingNoticesType =
    | All
    | Active;

  [@react.component]
  let make = (~children, ~palette, ~className="") => {
    let noticeSystem = use();
    let activeNotices = useNotices();
    let setShownNoticeType = useSetShownNoticeType();

    let getTransitionState =
      React.useCallback2(
        (notice: notice) =>
          {
            opacity:
              switch (noticeSystem.shownNoticesType, notice.isActive) {
              | (Active, true)
              | (All, _) => 1.
              | (Active, false) => 0.
              },
            transform: "translateY(0)",
            /*** Animating the height specifically allows the animation to be smooth when stacking notices. */
            height:
              switch (noticeSystem.shownNoticesType, notice.isActive) {
              | (Active, true)
              | (All, _) =>
                noticeSystem.noticeHeights
                ->getNoticeHeight(notice.id)
                ->Option.getWithDefault(0)
              | (Active, false) => 0
              },
          },
        (noticeSystem.noticeHeights, noticeSystem.shownNoticesType),
      );

    let transitions =
      TransitionHook.use(
        noticeSystem.notices,
        item => item.key,
        TransitionHook.config(
          ~from=
            _notice => {
              {opacity: 0., transform: "translateY(200%)", height: 0}
            },
          ~update=getTransitionState,
          ~enter=getTransitionState,
          ~leave=
            _notice => {{opacity: 0., transform: "translateY(0)", height: 0}},
          ~unique=true,
          ~reset=true,
          ~config=springConfig,
          (),
        ),
      );

    <>
      children
      <div className={Css.merge([Style.board(palette), className])}>
        <div className=Style.noticesContainer>
          {transitions
           ->Array.map(({props, key, item}) => {
               <Spring.Div key style={props->animatedValuesToStyle}>
                 <Item id={item.id}>
                   {item.element
                    ->ReasonReact.cloneElement(~props={"id": item.id}, [||])}
                 </Item>
               </Spring.Div>
             })
           ->React.array}
        </div>
        <div
          className={Style.toggleAllNotices(
            noticeSystem.notices->Array.length > 0
            && noticeSystem.notices->Array.length > activeNotices->Array.length,
          )}>
          <button
            onClick={_ =>
              setShownNoticeType(
                switch (noticeSystem.shownNoticesType) {
                | Active => All
                | All => Active
                },
              )
            }>
            (
              noticeSystem.shownNoticesType == All
                ? "Show Current Notices" : "Show All Notices"
            )
            ->React.string
          </button>
        </div>
      </div>
    </>;
  };
};

[@react.component]
let make =
    (
      ~children,
      ~noticeLife=defaultLife,
      ~palette=defaultPalette,
      ~globalNoticeContent=?,
      ~className="",
    ) => {
  let (value, dispatch) = React.useReducer(reducer, initialState);

  React.useEffect1(
    () => {
      dispatch->SetDispatch->dispatch;
      None;
    },
    [|dispatch|],
  );

  let value =
    React.useMemo3(
      () =>
        {
          ...value,
          noticeLife,
          globalNoticeContent:
            switch (globalNoticeContent) {
            | None => initialState.globalNoticeContent
            | Some(globalNoticeContent) => globalNoticeContent
            },
        },
      (value, globalNoticeContent, noticeLife),
    );

  <ContextProvider value>
    <Component palette className> children </Component>
  </ContextProvider>;
};

module TestButtons = {
  [@react.component]
  let make = () => {
    let addNotice = useAddNotice();
    <p>
      <button onClick={_ => addNotice(`error)->ignore}>
        "Create Error Notice"->React.string
      </button>
      <button onClick={_ => addNotice(~title="Loading", `loading)->ignore}>
        "Create Loading Notice"->React.string
      </button>
      <button
        onClick={_ =>
          addNotice(
            ~content="You did something that worked!"->React.string,
            `success,
          )
          ->ignore
        }>
        "Create Success Notice"->React.string
      </button>
      <button
        onClick={_ =>
          addNotice(~content="Some kind of info!"->React.string, `info)
          ->ignore
        }>
        "Create Info Notice"->React.string
      </button>
      <button
        onClick={_ =>
          addNotice(
            ~content="Warning about something.."->React.string,
            `warning,
          )
          ->ignore
        }>
        "Create Warning Notice"->React.string
      </button>
    </p>;
  };
};
