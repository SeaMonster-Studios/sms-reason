open Belt;

[@bs.deriving abstract]
type queryConfig('data, 'key, 'fnParamsFilter) = {
  [@bs.optional]
  retry: int,
  [@bs.optional]
  retryDelay: int,
  [@bs.optional]
  initialData: unit => 'data,
  [@bs.optional]
  suspense: bool,
  [@bs.optional]
  staleTime: int,
  [@bs.optional]
  cacheTime: int,
  [@bs.optional]
  refetchAllOnWindowFocus: bool,
  [@bs.optional]
  refetchInterval: bool,
  [@bs.optional]
  queryKeySerializerFn: 'key => unit,
  [@bs.optional]
  queryFnParamsFilter: 'fnParamsFilter => unit,
  [@bs.optional]
  throwOnError: bool,
  [@bs.optional]
  useErrorBoundary: Js.Nullable.t(bool),
  [@bs.optional]
  onSuccess: unit => unit,
  [@bs.optional]
  onError: unit => unit,
  [@bs.optional]
  onSettled: unit => unit,
  [@bs.optional]
  refetchOnMount: bool,
};

let makeConfig = queryConfig;

module ConfigProvider = {
  [@bs.module "react-query"] [@react.component]
  external make:
    (
      ~children: React.element,
      ~config: queryConfig('data, 'key, 'fnParamsFilter)
    ) =>
    React.element =
    "ReactQueryConfigProvider";
};

type key('k) = array('k);
type queryVariables('a) = array('a);
type queryFn('a, 'b) = 'a => Js.Promise.t('b);
type keyRec('a) = {key: array('a)};

type query('key, 'queryVariables, 'queryFn, 'data, 'fnParamsFilter) = {
  queryKey: keyRec('key),
  queryHash: string,
  queryVariables: array('queryVariables),
  queryFn: Js.Nullable.t('queryFn),
  config: queryConfig('data, 'key, 'fnParamsFilter),
};

type data('h, 'c, 'r) = {
  data: Js.Json.t,
  status: int,
  statusText: string,
  headers: Js.t('h),
  config: Js.t('c),
  request: Js.t('r),
};

type response(
  'key,
  'queryVariables,
  'queryFn,
  'data,
  'fnParamsFilter,
  'dataHeaders,
  'dataConfig,
  'dataRequest,
) = {
  status: string,
  error: Js.Nullable.t(string),
  isFetching: bool,
  canFetchMore: bool,
  failureCount: int,
  isStale: bool,
  isInactive: bool,
  data: data('dataHeaders, 'dataConfig, 'dataRequest),
  updateAt: int,
  refetch: unit => unit,
  query: query('key, 'queryVariables, 'queryFn, 'data, 'fnParamsFilter),
};

type status('data) =
  | NotAsked
  | Loading
  | Success('data)
  | Failure(string);

module Query = {
  [@bs.module "react-query"]
  external useQuery_:
    (
      Js.Nullable.t(key('key)),
      Js.Nullable.t(queryVariables('queryVariables)),
      queryFn('fa, 'fb),
      Js.Nullable.t(queryConfig('data, 'k, 'fnParamsFilter))
    ) =>
    response(
      'key,
      'queryVariables,
      'queryFn,
      'data,
      'fnParamsFilter,
      'dataHeaders,
      'dataConfig,
      'dataRequest,
    ) =
    "useQuery";

  let useQuery =
      (~key=?, ~variables=?, ~fn, ~config=?, decoder: Js.Json.t => 'decoded) => {
    let (status, setStatus) = React.useState(() => NotAsked);
    let query =
      useQuery_(
        switch (key) {
        | Some(key) => key->Js.Nullable.return
        | None => Js.Nullable.null
        },
        switch (variables) {
        | Some(variables) => variables->Js.Nullable.return
        | None => Js.Nullable.null
        },
        fn,
        switch (config) {
        | Some(config) => config->Js.Nullable.return
        | None => Js.Nullable.null
        },
      );

    React.useEffect4(
      () => {
        switch (query.status) {
        | "success" =>
          query.data.data
          ->decoder
          ->(
              fun
              | Result.Ok(data) => setStatus(_ => Success(data))
              | Result.Error(error) => {
                  Js.log4("Decoder error. query key:", key, "Error:", error);
                  setStatus(_ => Failure("Data error"));
                }
            )
        | "error" =>
          setStatus(_ =>
            Failure(
              query.error
              ->Js.Nullable.toOption
              ->(
                  fun
                  | Some(error) => error
                  | None => "Unknown error"
                ),
            )
          )
        | "loading" => setStatus(_ => Loading)
        | _ =>
          setStatus(_ => NotAsked);
          Js.log2("Unrecognized react query response status:", query.status);
        };

        None;
      },
      (setStatus, query.status, query.data, query.error),
    );
    (status, query);
  };
};

[@bs.module "react-query"]
external prefetchQuery:
  (
    key('k),
    queryVariables('variables),
    queryFn('fa, 'fb),
    queryConfig('data, 'key, 'fnParamsFilter)
  ) =>
  Js.Promise.t('data) =
  "prefetchQuery";