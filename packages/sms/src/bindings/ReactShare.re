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

module Facebook = {
  [@bs.module "react-share"] [@react.component]
  external make:
    (
      ~children: React.element,
      ~url: string,
      ~quote: string=?,
      ~ariaLabel: string=?
    ) =>
    React.element =
    "FacebookShareButton";
};