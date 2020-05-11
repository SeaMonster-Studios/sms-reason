module Sms = SeamonsterStudiosReason;

type shownNoticesType =
  | Active
  | All;

type noticeColor = {
  text: Css.Types.Color.t,
  accent: Css.Types.Color.t,
  background: Css.Types.Color.t,
};

type noticeColors = {
  info: noticeColor,
  success: noticeColor,
  warning: noticeColor,
  error: noticeColor,
};

type toggleNoticeTypeButton = {
  background: Css.Types.Color.t,
  text: Css.Types.Color.t,
};

type palette = {
  notices: noticeColors,
  toggleNoticeTypeButton,
};

let defaultPalette = {
  notices: {
    info: {
      text: Css.rgb(61, 128, 204),
      accent: Css.rgb(77, 100, 128),
      background: Css.rgb(152, 201, 255),
    },
    success: {
      text: Css.rgb(52, 199, 125),
      accent: Css.rgb(69, 122, 95),
      background: Css.rgb(140, 251, 196),
    },
    warning: {
      text: Css.rgb(204, 185, 61), // 255, 255, 51
      accent: Css.rgb(128, 121, 77),
      background: Css.rgb(255, 241, 153),
    },
    error: {
      text: Css.rgb(204, 84, 57),
      accent: Css.rgb(128, 84, 74),
      background: Css.rgb(255, 168, 148),
    },
  },
  toggleNoticeTypeButton: {
    text: Css.rgb(255, 255, 255),
    background: Css.rgb(61, 128, 204),
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
    style([
      label("notice--board"),
      position(`fixed),
      bottom(0->px),
      right(0->px),
      zIndex(99999999),
      display(`flex),
      flexDirection(`column),
      padding(20->px),
      selector(
        "[class*='notice--info']",
        [
          borderLeftColor(p.notices.info.accent),
          color(p.notices.info.text),
          backgroundColor(p.notices.info.background),
          selector(
            "button[class*='remove-button'] path",
            [SVG.fill(p.notices.info.text)],
          ),
          selector(
            "div[class*='life-bar'] > div",
            [backgroundColor(p.notices.info.text)],
          ),
        ],
      ),
      selector(
        "[class*='notice--success']",
        [
          borderLeftColor(p.notices.success.accent),
          color(p.notices.success.text),
          backgroundColor(p.notices.success.background),
          selector(
            "button[class*='remove-button'] path",
            [SVG.fill(p.notices.success.text)],
          ),
          selector(
            "div[class*='life-bar'] > div",
            [backgroundColor(p.notices.success.text)],
          ),
        ],
      ),
      selector(
        "[class*='notice--warning']",
        [
          borderLeftColor(p.notices.warning.accent),
          color(p.notices.warning.text),
          backgroundColor(p.notices.warning.background),
          selector(
            "button[class*='remove-button'] path",
            [SVG.fill(p.notices.warning.text)],
          ),
          selector(
            "div[class*='life-bar'] > div",
            [backgroundColor(p.notices.warning.text)],
          ),
        ],
      ),
      selector(
        "[class*='notice--error']",
        [
          borderLeftColor(p.notices.error.accent),
          color(p.notices.error.text),
          backgroundColor(p.notices.error.background),
          selector(
            "button[class*='remove-button'] path",
            [SVG.fill(p.notices.error.text)],
          ),
          selector(
            "div[class*='life-bar'] > div",
            [backgroundColor(p.notices.error.text)],
          ),
        ],
      ),
      selector(
        "[class*='notices--type-toggle']",
        [
          backgroundColor(p.toggleNoticeTypeButton.background),
          selector("button", [color(p.toggleNoticeTypeButton.text)]),
        ],
      ),
      selector(
        "[class*='notice--loader'] > div > div",
        [background(p.notices.info.text)],
      ),
    ]);

  let noticesContainer =
    style([
      label("notices--container"),
      overflowY(`auto),
      display(`flex),
      flexDirection(`column),
      maxHeight(`calc((`sub, 100.->vh, 20->px))),
      selector("> div", [overflow(`hidden)]),
    ]);

  let toggleAllNotices = visible =>
    style([
      label("notices--type-toggle"),
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
    ]);

  module Notice = {
    let base =
      merge([
        style([
          label("notices--notice"),
          width(275->px),
          backgroundColor(Common.white),
          Common.boxShadow,
          borderLeftWidth(4->px),
          borderLeftStyle(`solid),
          display(`flex),
          flexDirection(`column),
          marginTop(10->px),
        ]),
      ]);

    let info = merge([base, style([label("notice--info")])]);

    let success = merge([base, style([label("notice--success")])]);

    let warning = merge([base, style([label("notice--warning")])]);

    let error = merge([base, style([label("notice--error")])]);

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
        style([
          width(100.->pct),
          marginTop(10->px),
          label("notice--life-bar"),
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
      style([
        label("notice--header"),
        padding3(~bottom=10->px, ~top=10->px, ~h=10->px),
        textTransform(`uppercase),
        fontWeight(`bold),
        fontSize(12->px),
        display(`flex),
        alignItems(`center),
        justifyContent(`spaceBetween),
      ]);

    let content =
      style([
        label("notice--content"),
        padding3(~top=0->px, ~bottom=10->px, ~h=10->px),
        fontSize(14->px),
        lineHeight(1.25->em),
      ]);

    let loaderContent =
      style([
        label("notice--loader-content"),
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
          "div[class*='life-bar']",
          [margin(0->px), padding3(~bottom=10->px, ~h=20->px, ~top=0->px)],
        ),
      ]);

    let loader =
      style([
        label("notice--loader"),
        transforms([scale(0.5, 0.5)]),
        selector(".inner-container", [margin(0->px)]),
      ]);

    let removeButton = shownNoticeType =>
      merge([
        hideWithAllNoticeType(shownNoticeType),
        style([
          label("notice--remove-button"),
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

type noticeSystemState = {
  notices: array(notice),
  noticeHeights: Belt.Map.String.t(int),
  noticeLife: int, //milliseconds
  shownNoticesType,
};

type noticeSystemAction =
  | AddNotice(string, string, newNotice)
  | SetNoticeHeight(setNoticeHeight)
  | RemoveNotice(string)
  | SetNoticeLife(int)
  | SetShownNoticeType(shownNoticesType);

let reducer = (state: noticeSystemState, action) =>
  switch (action) {
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

let noticeSystemStore =
  Reductive.Store.create(
    ~reducer,
    ~preloadedState={
      notices: [||],
      noticeHeights: Map.String.empty,
      noticeLife: defaultLife,
      shownNoticesType: Active,
    },
    (),
  );

module Store = {
  include ReductiveContext.Make({
    type action = noticeSystemAction;
    type state = noticeSystemState;
  });
};

let noticeHeightsSelector = state => state.noticeHeights;

let useNoticeHeights = () => Store.useSelector(noticeHeightsSelector);

let getNoticeHeight = (noticeHeights, id) =>
  noticeHeights->Map.String.get(id);

let useNoticeHeight = id => {
  let noticeHeights = useNoticeHeights();
  React.useMemo2(
    () => noticeHeights->getNoticeHeight(id),
    (noticeHeights, id),
  );
};

let allNoticesSelector = state => state.notices;

let useAllNotices = () => Store.useSelector(allNoticesSelector);

let shownNoticeTypeSelector = state => state.shownNoticesType;

let useShownNoticeType = () => Store.useSelector(shownNoticeTypeSelector);

let noticeLifeSelector = state => state.noticeLife;

let useNoticeLife = () => Store.useSelector(noticeLifeSelector);

let useNotices = () => {
  let allNotices = useAllNotices();

  React.useMemo1(
    () => allNotices->Array.keep(notice => notice.isActive),
    [|allNotices|],
  );
};

let useNotice = id => {
  let notices = useAllNotices();

  React.useMemo2(
    () => notices->Array.getBy(notice => notice.id == id),
    (notices, id),
  );
};

let useSetShownNoticeType = () => {
  let dispatch = Store.useDispatch();

  React.useCallback1(
    shownNoticesType => {dispatch(SetShownNoticeType(shownNoticesType))},
    [|dispatch|],
  );
};

let useRemoveNotice = () => {
  let dispatch = Store.useDispatch();

  React.useCallback1(
    noticeId => {dispatch(RemoveNotice(noticeId))},
    [|dispatch|],
  );
};

module Notice = {
  module Header = {
    [@react.component]
    let make = (~id, ~title) => {
      let shownNoticeType = useShownNoticeType();
      let notice = useNotice(id);
      let removeNotice = useRemoveNotice();

      <header className=Style.Notice.header>
        <div> title->React.string </div>
        {switch (shownNoticeType) {
         | Active =>
           <button
             className={Style.Notice.removeButton(shownNoticeType)}
             type_="button"
             onClick={_ =>
               switch (notice) {
               | None => ()
               | Some(notice) => removeNotice(notice.key)
               }
             }>
             <Icons.Close />
           </button>
         | All => React.null
         }}
      </header>;
    };
  };

  /**id props on this component defaults to "", but the NoticeSystem always passes the id to the component. The user will not need to do this when instantiating this */
  [@react.component]
  let make = (~title=?, ~content, ~id="") => {
    let notice = useNotice(id);
    let removeNotice = useRemoveNotice();
    let shownNoticeType = useShownNoticeType();
    // let logout = AppStoreHooks.useLogout();

    let life =
      React.useMemo2(
        () => {
          switch (shownNoticeType, notice) {
          | (All, _) => 999999999
          | (Active, Some(notice)) => notice.life
          | (Active, _) => defaultLife
          }
        },
        (shownNoticeType, notice),
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

    let countdown = ReactUseCountdown.useHook(life, handleRemoveNotice);

    React.useEffect1(
      () => {
        countdown.play();
        None;
      },
      [|countdown.play|],
    );

    let timerNode =
      switch (shownNoticeType) {
      | Active =>
        <div
          className={Style.Notice.life(
            life,
            countdown.playState,
            shownNoticeType,
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
        {switch (notice.type_) {
         | `loading =>
           <div className=Style.Notice.loaderContent>
             <div>
               <Loaders.Container className=Style.Notice.loader>
                 <Sms.ReactSpinners.PropagateLoader size=20 loading=true />
               </Loaders.Container>
             </div>
             timerNode
           </div>
         | _ =>
           <div className=Style.Notice.content>
             content
             //  {switch (notice.type_) {
             //   | `error =>
             //     <div>
             //       <div>
             //         "An unknown error has occurred, please try again later. If this continues, please contact us."
             //         ->React.string
             //       </div>
             //     </div>
             //   // <a
             //   //   href="/contact"
             //   //   rel="noopener noreferrer"
             //   //   target="_blank">
             //   //   "here"->React.string
             //   // </a>
             //   /***TODO: Implement a way for a custom message here. Ideally we'd want a developer to be able to implement a sentry "send report" form and maybe give the user an opportunity to restart the app */
             //   | _ => React.null
             //   }}
             timerNode
           </div>
         }}
      </div>
    | None => React.null
    };
  };
};

let useAddNoticeAndGetId = () => {
  let dispatch = Store.useDispatch();

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
        | (None, None) =>
          <Notice content={<span> "test"->React.string </span>} ?title />
        };

      let id =
        switch (id_) {
        | Some(id) => id
        | None => Sms.Uuid.make()
        };

      dispatch(AddNotice(id, key, (type_, element, life)));
      id;
    },
    [|dispatch|],
  );
};

type manage = {
  add:
    (
      ~id: string=?,
      ~content: React.element=?,
      ~el: React.element=?,
      ~title: string=?,
      ~life: int=?,
      type_
    ) =>
    string,
  remove: string => unit,
};

let useManage = () => {
  let add = useAddNoticeAndGetId();
  let remove = useRemoveNotice();

  React.useMemo2(() => {add, remove}, (add, remove));
};

let useAddNotice = () => {
  let dispatch = Store.useDispatch();

  React.useMemo1(
    ((), ~id, ~content=?, ~el=?, ~title=?, ~life: singleNoticeLife=?, type_) => {
      /** If an id is provided then this new notice will _replace_ the previous notice with that id. This is beneficial when using notices for acync states (Loading, Success, Error) */
      let key = Sms.Uuid.make();
      let element =
        switch (el, content) {
        | (Some(el), _) => el
        | (None, Some(content)) => <Notice content ?title />
        | (None, None) =>
          <Notice content={<span> "test"->React.string </span>} ?title />
        };

      dispatch(AddNotice(id, key, (type_, element, life)));
    },
    [|dispatch|],
  );
};

type useManageNoticeValue = {
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

let useManageNotice = () => {
  let noticeId = React.useMemo0(() => Sms.Uuid.make());
  let addNotice = useAddNotice();
  let removeNotice = useRemoveNotice();
  let notice = useNotice(noticeId);

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

let useSetNoticeHeight = () => {
  let dispatch = Store.useDispatch();

  React.useCallback1(
    setNoticeHeight => {dispatch(SetNoticeHeight(setNoticeHeight))},
    [|dispatch|],
  );
};

let useSetNoticeLife = () => {
  let dispatch = Store.useDispatch();

  React.useCallback1(
    noticeLife => {dispatch(SetNoticeLife(noticeLife))},
    [|dispatch|],
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
  let make = (~children, ~noticeLife, ~palette, ~className="") => {
    let activeNotices = useNotices();
    let allNotices = useAllNotices();
    let setNoticeLife = useSetNoticeLife();
    let showNoticeType = useShownNoticeType();
    let setShownNoticeType = useSetShownNoticeType();
    let noticeHeights = useNoticeHeights();

    React.useEffect2(
      () => {
        setNoticeLife(noticeLife);
        None;
      },
      (setNoticeLife, noticeLife),
    );

    let getTransitionState =
      React.useCallback2(
        (notice: notice) =>
          {
            opacity:
              switch (showNoticeType, notice.isActive) {
              | (Active, true)
              | (All, _) => 1.
              | (Active, false) => 0.
              },
            transform: "translateY(0)",
            /*** Animating the height specifically allows the animation to be smooth when stacking notices. */
            height:
              switch (showNoticeType, notice.isActive) {
              | (Active, true)
              | (All, _) =>
                noticeHeights
                ->getNoticeHeight(notice.id)
                ->Option.getWithDefault(0)
              | (Active, false) => 0
              },
          },
        (noticeHeights, showNoticeType),
      );

    let transitions =
      TransitionHook.use(
        allNotices,
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
            allNotices->Array.length > 0
            && allNotices->Array.length > activeNotices->Array.length,
          )}>
          <button
            onClick={_ =>
              setShownNoticeType(
                switch (showNoticeType) {
                | Active => All
                | All => Active
                },
              )
            }>
            (
              showNoticeType == All
                ? "Show Current Notices" : "Show All Notices"
            )
            ->React.string
          </button>
        </div>
      </div>
    </>;
  };
};

module Container = {
  [@react.component]
  let make =
      (
        ~children,
        ~noticeLife=defaultLife,
        ~palette=defaultPalette,
        ~className="",
      ) =>
    <Store.Provider store=noticeSystemStore>
      <Component palette className noticeLife> children </Component>
    </Store.Provider>;
};