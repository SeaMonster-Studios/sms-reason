# React Query bindings

JS Lib documentation: https://github.com/tannerlinsley/react-query

Install:
`yarn add @seamonster-studios/bs-react-query`

Add `@seamonster-studios/bs-react-query` to your bsconfig.json

Example Usage:
```reason

let reportPromiseError = (error: Js.Promise.error) => {
  Js.log(error);
  SentryBrowser.capturePromiseException(error);
};

let reportDecodeError = (error: Decco.decodeError, decoderName) => {
  Js.log(error);
  SentryBrowser.captureDeccoError(error, decoderName);
};


let promiseQueryPrep = (promise, decoder, fnName, decoderName) =>
  promise 
  ->FutureJs.fromPromise(_error => () /** If an async call is being handled by react query , we are already reporting the error via ReactQuery.ConfigProvider so we don't need to report it again when converting our promise to a future */)
  ->Future.mapOk(res => res##data->decoder)
  ->Future.tapOk(decoded =>
      switch (decoded) {
      | Ok(_) => ()
      | Error(decodeError) =>
        decodeError->reportDecodeError({j|$fnName: $decoderName|j})
      }
    )
  ->FutureJs.resultToPromise;

let useProject = projectId =>
  ReactQuery.Hooks.useQuery(~key=("projects", [|projectId|]), (_, _, _) =>
    Api.instance
    ->Axios.Instance.get("/projects/" ++ projectId)
    ->promiseQueryPrep(project_decode, "useProject", "project_decode")
  );

<ReactQuery.ConfigProvider
  config=ReactQuery.ConfigProvider.(
    config(
      ~cacheTime=5000,
      ~onError=error => error->Utils.reportPromiseError,
      (),
    )
  )
>
  // ...App
</ReactQuery.ConfigProvider>

```