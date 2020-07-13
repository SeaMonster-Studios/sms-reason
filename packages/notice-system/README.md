# NoticeSystem

Install:
`yarn add @seamonster-studios/notice-system`

Install Peer Dependencies:

`yarn add @seamonster-studios/react-use-countdown @seamonster-studios/react-use-measure @seamonster-studios/reason bs-css-emotion bs-react-spring`

## Features
- Focus on async request status
- Life bar
- Life bar pause on hover
- View all previous notices

![Demo](https://opensource-repo-assets.s3-us-west-2.amazonaws.com/reason/notice-system/demo.gif)


## Basic Usage

### Setup provider component:

```reason
<NoticeSystem>
// ... app
</NoticeSystem>
```

Adding a custom color palette or default content for each of the notice system types (both optional):

```reason


let globalNoticeContent: NoticeSystem.globalNoticeContent = {
  success: None,
  loading: Some((<NoticeSystem.LoaderContent />, `replace)),
  warning: None,
  info: None,
  error:
    Some((
      <p>
        "Our development team has been notified. Please try again later."
        ->React.string
      </p>,
      `after,
    )),
};

let palette: NoticeSystem.palette =
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

// ...

<NoticeSystem globalNoticeContent palette>
  // ... app
</NoticeSystem>

```

### Hooks

- `use()` Returns the entire value of the Notice System
- `useNew()` Is the bread and butter hook. When instantiating the hook it creates a new notice without activating it. It then returns a record of methods and values to use. Most notably `add` and `remove`. This is helpful b/c you can use a single notice for all API states of an async request (loading, success, error).
- `useAddNotice()` Lets you create any number of notices. It returns to you the notice `id` upon doing so.
- `useNotices()` Returns an array of all the notices
- `useNotice(noticeId)` Returns None or Some(notice)
- `useSetShownNoticeType` Lets you control if you want to show only the `Active` notices, or `All` of them  
- `useRemoveNotice()` Lets you inactivate a notice by id

### To add a notice (via useNew or useAddNotice)
```reason
// Type
type add = (
  ~content: ReasonReact.reactElement=?,
  ~el: React.element=?,
  ~title: string=?,
  ~life: int=?,
  type_
) =>
unit;

// Usage
let myAsyncRequest = (~entity) => {
  notice.add(~title=entity, `loading);

  Js.Promise.(your_promise())
  |> then_(res => {
    switch(res) {
      | Ok(data) => {
        // ...
        notice.add(~content="Your success message here"->React.string, `success)
        // .. OR ... if the UI is self evident when the response was successful
        notice.remove()
      }
      | Error(error) => {
        // ...
        notice.add(~content="Your error message here"->React.string, `error)
      }
    }
  })
};

```
