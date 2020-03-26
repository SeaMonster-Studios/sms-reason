module Style = {
  open Css;

  let content =
    style([
      label("react-copy-to-clipboard"),
      cursor(`pointer),
      borderStyle(`none),
      display(`inlineFlex),
      alignItems(`center),
      selector("svg", [marginRight(4->px)]),
      selector(
        "&:focus",
        [outline(px(1), `solid, rgba(44, 42, 41, 0.25))],
      ),
    ]);

  let action =
    style([
      label("action"),
      marginLeft(9->px),
      color(rgb(97, 92, 90)),
      display(`flex),
      alignItems(`center),
    ]);
};

module IconCopy = {
  [@react.component]
  let make = (~className="") =>
    <svg
      className
      width="11"
      height="14"
      viewBox="0 0 11 14"
      fill="none"
      xmlns="http://www.w3.org/2000/svg">
      <path
        d="M8.97368 12.25H0.289474C0.129684 12.25 0 12.1193 0 11.9583V3.79167C0 3.71408 0.0306842 3.64 0.0845263 3.58517L3.55821 0.0851667C3.61263 0.0309167 3.68616 0 3.76316 0H8.97368C9.13347 0 9.26316 0.130667 9.26316 0.291667V11.9583C9.26316 12.1193 9.13347 12.25 8.97368 12.25ZM0.578947 11.6667H8.68421V0.583333H3.883L0.578947 3.91242V11.6667Z"
        fill="#0072CE"
      />
      <path
        d="M3.76316 4.08333H0.289474C0.129684 4.08333 0 3.95267 0 3.79167C0 3.63067 0.129684 3.5 0.289474 3.5H3.47368V0.291667C3.47368 0.130667 3.60337 0 3.76316 0C3.92295 0 4.05263 0.130667 4.05263 0.291667V3.79167C4.05263 3.95267 3.92295 4.08333 3.76316 4.08333Z"
        fill="#0072CE"
      />
      <path
        d="M8.97368 12.25H0.289474C0.129684 12.25 0 12.1193 0 11.9583V10.2083C0 10.0473 0.129684 9.91667 0.289474 9.91667C0.449263 9.91667 0.578947 10.0473 0.578947 10.2083V11.6667H8.68421V0.583333H7.23684C7.07705 0.583333 6.94737 0.452667 6.94737 0.291667C6.94737 0.130667 7.07705 0 7.23684 0H8.97368C9.13347 0 9.26316 0.130667 9.26316 0.291667V11.9583C9.26316 12.1193 9.13347 12.25 8.97368 12.25Z"
        transform="translate(1.73682 1.75)"
        fill="#0072CE"
      />
    </svg>;
};

[@react.component]
let make = (~text, ~className="", ~actionClassName="", ~icon={<IconCopy />}) => {
  let (copied, setCopied) = React.useState(() => false);
  React.useEffect2(
    () => {
      let timeoutId =
        Js.Global.setTimeout(
          () =>
            if (copied) {
              setCopied(_ => false);
            },
          3000,
        );
      Some(() => timeoutId->Js.Global.clearTimeout);
    },
    (copied, setCopied),
  );
  <Sms.ReactCopyToClipboard text onCopy={() => setCopied(_ => true)}>
    <div className={Css.merge([Style.content, className])}>
      <span> text->React.string </span>
      <span className={Css.merge([Style.action, actionClassName])}>
        {copied
           ? "copied!"->React.string : <> icon "copy"->React.string </>}
      </span>
    </div>
  </Sms.ReactCopyToClipboard>;
};