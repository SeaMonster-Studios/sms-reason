open Utils;
open Document;

type element;
[@bs.val] [@bs.return nullable] [@bs.scope "document"]
external getElementById: string => option(element) = "getElementById";

let makeContainer = () => {
  Document.appendChild(Document.createElement("div"))->ignore;
};

type mount = {
  id: string,
  node: React.element,
};

module Mount1 = {
  [@react.component]
  let make = () =>
    <>
      <NoticeSystem.Container>
        <h1> "Hello World"->str </h1>
      </NoticeSystem.Container>
    </>;
};
let mounts = [|{id: "react-mount-1", node: <Mount1 />}|];

Array.map(
  mount =>
    switch (getElementById(mount.id)) {
    | None => ()
    | Some(_el) => ReactDOMRe.renderToElementWithId(mount.node, mount.id)
    },
  mounts,
);