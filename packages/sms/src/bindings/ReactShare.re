module Twitter = {
  [@bs.module "react-share"] [@react.component]
  external make:
    (
      ~children: React.element,
      ~url: string,
      ~title: string=?,
      ~via: string=?,
      ~hashtags: array(string)=?,
      ~ariaLabel: string=?
    ) =>
    React.element =
    "TwitterShareButton";
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
      (~children, ~url: string, ~quote: option(string)=?, ~ariaLabel: string) =>
    <Spread props={"aria-label": ariaLabel}>
      <Facebook0 url ?quote> children </Facebook0>
    </Spread>;
};