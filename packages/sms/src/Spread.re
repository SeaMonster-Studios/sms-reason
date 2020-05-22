/* https://reasonml.github.io/reason-react/docs/en/adding-data-props */ 
[@react.component]
let make = (~props, ~children) => ReasonReact.cloneElement(children, ~props, [||]);