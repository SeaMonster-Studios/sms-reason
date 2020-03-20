module Style = {
  open Css;

  let container =
    style([
      display(`flex),
      justifyContent(`center),
      marginTop(35->px),
      marginBottom(35->px),
    ]);
};

module Container = {
  [@react.component]
  let make = (~children: React.element, ~className="") => {
    <div className={Css.merge([Style.container, className])}> children </div>;
  };
};