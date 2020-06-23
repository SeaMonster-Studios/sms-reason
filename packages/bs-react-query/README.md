# React Query bindings

JS Lib documentation: https://github.com/tannerlinsley/react-query

Install:
`yarn add @seamonster-studios/bs-react-query @seamonster-studios/bs-lodash @seamonster-studios/react-hooks`

Add `@seamonster-studios/bs-react-query` to your bsconfig.json

Example Usage:
```reason

// App.re
[@react.component]
let make = () => {
  let queryConfig =
      React.useMemo0(() =>
        ReactQuery.ConfigProvider.(
          config(
            // ~cacheTime=5000,
            // ~staleTime=10000,
            ~onError=error => error->Utils.reportPromiseError,
            (),
          )
        )
      );

  <ReactQuery.ConfigProvider config=queryConfig>
    // ...children
  </ReactQuery.ConfigProvider>
}

// AuthHooks.re

include ReactQuery.QuerySet({
  type key = string;
  type data = Result.t(Auth.authResponseData, Decco.decodeError);
  type queryVars = unit;
  type mutateVars = Auth0.user;
});

let endpoint = "/login";

let useAuthed = Query.use(~key=ready ? endpoint->Some : None, ~options?, (key, _) =>
  // return a promise with value with the same type as data
);

let useLogin = () => Mutation.use(_ => {
  // return a promise with value with the same type as data
})


// Root.re
[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();

  let authed =
    AuthHooks.useAuthed(
      ~options=AuthHooks.Query.Options.(make(~retry=0, ())),
      (),
    );

  {switch (authed.status, authed.isFetching) {
    | (Success(Ok(authedStatus)), _) =>
      <SiteLayout authed=true>
        <RootAuthed authedStatus documentReferrer />
      </SiteLayout>
    | (Loading, true) => <Loaders.SplashScreen />
    | (Loading, false)
    | (Success(Error(_)), _)
    | (Error(_), _) =>
      <SiteLayout authed=false>
        <MarketingLayout>
          {switch (url.path) {
            //  screens by path
          }}
        </MarketingLayout>
      </SiteLayout>
    }}
};

```
