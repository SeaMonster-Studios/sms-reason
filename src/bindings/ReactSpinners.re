module PropagateLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (~size: int=?, ~color: string=?, ~loading: bool) => React.element =
    "PropagateLoader";
};