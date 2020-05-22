module Twitter0 = {
  [@bs.module "react-share"] [@react.component]
  external make:
    (
      ~children: React.element,
      ~url: string,
      ~title: string=?,
      ~via: string=?,
      ~hashtags: array(string)=?
    ) =>
    React.element =
    "TwitterShareButton";
};

module Twitter = {
  [@react.component]
  let make =
      (
        ~children,
        ~url,
        ~title=?,
        ~via=?,
        ~hashtags=?,
        ~ariaLabel: option(string)=?,
      ) => {
    let props = {
      "aria-label": ariaLabel->Belt.Option.getWithDefault("Twitter Share"),
    };
    <Spread props>
      <Twitter0 url ?title ?via ?hashtags> children </Twitter0>
    </Spread>;
  };
};

module Facebook0 = {
  [@bs.module "react-share"] [@react.component]
  external make:
    (~children: React.element, ~url: string, ~quote: string=?) => React.element =
    "FacebookShareButton";
};

module Facebook = {
  [@react.component]
  let make =
      (
        ~children,
        ~url,
        ~quote=?,
        ~ariaLabel: option(string)=?,
      ) => {
    let props = {
      "aria-label": ariaLabel->Belt.Option.getWithDefault("Facebook Share"),
    };
    <Spread props> <Facebook0 url ?quote> children </Facebook0> </Spread>;
  };
};