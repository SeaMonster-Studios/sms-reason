// Lib: https://github.com/tannerlinsley/react-query
// Typescript type reference: https://github.com/tannerlinsley/react-query/blob/master/types/index.d.ts
open Belt;

type key('keyVars) = (string, 'keyVars);

module Hooks = {
  module Options = {
    type manual = bool;
    type retry = int;
    type retryDelay = int => int;
    type staleTime = int;
    type cacheTime = int;
    type refetchInterval = int;
    type refetchIntervalInBackground = bool;
    type refetchOnWindowFocus = bool;
    type onSuccess('response) = 'response => unit;
    type onError = Js.Promise.error => unit;
    type onSettled('response) = ('response, Js.Promise.error) => unit;
    type suspense = bool;
    type initialData('response) = 'response;
    type refetchOnMount = bool;
    type queryFnParamsFilter('a) = 'a => 'a;
    type getFetchMore('response) = ('response, array('response)) => bool;

    [@bs.deriving abstract]
    type query('queryFnParamsFilter, 'response) = {
      [@bs.optional]
      manual,
      [@bs.optional]
      retry,
      [@bs.optional]
      retryDelay,
      [@bs.optional]
      staleTime,
      [@bs.optional]
      cacheTime,
      [@bs.optional]
      refetchInterval,
      [@bs.optional]
      refetchIntervalInBackground,
      [@bs.optional]
      refetchOnWindowFocus,
      [@bs.optional]
      onSuccess: onSuccess('response),
      [@bs.optional]
      onError,
      [@bs.optional]
      onSettled: onSettled('response),
      [@bs.optional]
      suspense,
      [@bs.optional]
      initialData: initialData('response),
      [@bs.optional]
      refetchOnMount,
      [@bs.optional]
      queryFnParamsFilter: queryFnParamsFilter('queryFnParamsFilter),
    };

    [@bs.deriving abstract]
    type paginatedQuery('response) = {
      [@bs.optional]
      manual,
      [@bs.optional]
      retry,
      [@bs.optional]
      retryDelay,
      [@bs.optional]
      staleTime,
      [@bs.optional]
      cacheTime,
      [@bs.optional]
      refetchInterval,
      [@bs.optional]
      refetchIntervalInBackground,
      [@bs.optional]
      refetchOnWindowFocus,
      [@bs.optional]
      onSuccess: onSuccess('response),
      [@bs.optional]
      onError,
      [@bs.optional]
      suspense,
      [@bs.optional]
      initialData: initialData('response),
      [@bs.optional]
      refetchOnMount,
    };

    [@bs.deriving abstract]
    type infiniteQuery('response) = {
      [@bs.optional]
      getFetchMore: getFetchMore('response),
      [@bs.optional]
      manual,
      [@bs.optional]
      retry,
      [@bs.optional]
      retryDelay,
      [@bs.optional]
      staleTime,
      [@bs.optional]
      cacheTime,
      [@bs.optional]
      refetchInterval,
      [@bs.optional]
      refetchIntervalInBackground,
      [@bs.optional]
      refetchOnWindowFocus,
      [@bs.optional]
      onSuccess: onSuccess('response),
      [@bs.optional]
      onError,
      [@bs.optional]
      suspense,
      [@bs.optional]
      initialData: initialData('response),
      [@bs.optional]
      refetchOnMount,
    };

    [@bs.deriving abstract]
    type mutate('response, 'mutateVars, 'mutateData) = {
      [@bs.optional]
      onMutate: 'mutateVars => Js.Promise.t('response),
      [@bs.optional]
      onSuccess:
        ('mutateData, 'mutateVars) => Js.Nullable.t(Js.Promise.t(unit)),
      [@bs.optional]
      onError:
        (Js.Promise.error, 'mutateVars, 'response) =>
        Js.Nullable.t(Js.Promise.t(unit)),
      [@bs.optional]
      onSettled:
        ('mutateData, Js.Promise.error, 'mutateVars, 'response) =>
        Js.Nullable.t(Js.Promise.t(unit)),
      [@bs.optional]
      throwOnError: bool,
      [@bs.optional]
      useErrorBoundary: bool,
      [@bs.optional]
      manual,
      [@bs.optional]
      retry,
      [@bs.optional]
      retryDelay,
      [@bs.optional]
      staleTime,
      [@bs.optional]
      cacheTime,
      [@bs.optional]
      refetchInterval,
      [@bs.optional]
      refetchIntervalInBackground,
      [@bs.optional]
      refetchOnWindowFocus,
      [@bs.optional]
      suspense,
      [@bs.optional]
      initialData: initialData('response),
      [@bs.optional]
      refetchOnMount,
    };
  };

  module Result = {
    type error_ = Js.Nullable.t(Js.Promise.error);
    type data_('response) = Js.Nullable.t('response);
    type isFetching = bool;
    type isStale = bool;
    type failureCount = int;
    type refetch = unit => unit;
    type fetchMore = unit => unit;
    type canFetchMore = bool;

    type mutate('response, 'mutateVars, 'mutateData) =
      (
        ~vars: 'mutateVars,
        ~options: Options.mutate('response, 'mutateVars, 'mutateData)=?,
        unit
      ) =>
      Js.Promise.t('response);

    type useMutation_unmodified_rec('response) = {
      status: string,
      data: data_('response),
      error: error_,
      promise: Js.Promise.t('response),
    };

    type useMutation_unmodified('response, 'mutateVars, 'mutateData) = (
      mutate('response, 'mutateVars, 'mutateData),
      useMutation_unmodified_rec('response),
    );

    type useMutation_rec('response) = {
      status: [
        | `idle
        | `loading
        | `error(Js.Promise.error)
        | `success('response)
      ],
      promise: Js.Promise.t('response),
    };

    type useMutation('response, 'mutateVars, 'mutateData) = (
      mutate('response, 'mutateVars, 'mutateData),
      useMutation_rec('response),
    );

    let convertMutation:
      useMutation_unmodified('response, 'mutateVars, 'mutateData) =>
      useMutation('response, 'mutateVars, 'mutateData) =
      ((mutate, result)) => {
        (
          mutate,
          {
            status:
              switch (
                result.status,
                result.data->Js.Nullable.toOption,
                result.error->Js.Nullable.toOption,
              ) {
              | ("success", Some(data), _) => `success(data)
              | ("error", _, Some(error)) => `error(error)
              | ("loading", _, _) => `loading
              | ("idle", _, _)
              | _ => `idle
              },
            promise: result.promise,
          },
        );
      };

    type useQuery_unmodified('response) = {
      status: string,
      error: error_,
      isFetching,
      isStale,
      failureCount,
      refetch,
      data: data_('response),
    };

    type useQuery('response) = {
      status: [ | `loading | `error(Js.Promise.error) | `success('response)],
      isFetching,
      isStale,
      refetch,
      failureCount,
    };

    let convertQuery: useQuery_unmodified('response) => useQuery('response) =
      result => {
        {
          status:
            switch (
              result.status,
              result.data->Js.Nullable.toOption,
              result.error->Js.Nullable.toOption,
            ) {
            | ("success", Some(data), _) => `success(data)
            | ("error", _, Some(error)) => `error(error)
            | ("loading", _, _)
            | _ => `loading
            },
          isFetching: result.isFetching,
          isStale: result.isStale,
          failureCount: result.failureCount,
          refetch: result.refetch,
        };
      };

    type usePaginatedQuery_unmodified('response) = {
      status: string,
      resolvedData: data_('response),
      latestData: data_('response),
      error: error_,
      isFetching,
      isStale,
      failureCount,
      refetch,
    };

    type usePaginatedQuery('response) = {
      status: [
        | `loading
        | `error(Js.Promise.error)
        | `success('response, 'response)
      ],
      isFetching,
      isStale,
      failureCount,
      refetch,
    };

    let convertPaginated:
      usePaginatedQuery_unmodified('response) => usePaginatedQuery('response) =
      result => {
        status:
          switch (
            result.status,
            result.resolvedData->Js.Nullable.toOption,
            result.latestData->Js.Nullable.toOption,
            result.error->Js.Nullable.toOption,
          ) {
          | ("success", Some(resolvedData), Some(latestData), _) =>
            `success((resolvedData, latestData))
          | ("success", None, Some(data), _)
          | ("success", Some(data), None, _) => `success((data, data))
          | ("error", _, _, Some(error)) => `error(error)
          | ("loading", _, _, _)
          | _ => `loading
          },
        isFetching: result.isFetching,
        isStale: result.isStale,
        failureCount: result.failureCount,
        refetch: result.refetch,
      };

    type useInfiniteQuery_unmodified('response) = {
      status: string,
      data: data_('response),
      error: error_,
      isFetching,
      isStale,
      failureCount,
      refetch,
      fetchMore,
      canFetchMore,
    };

    type useInfiniteQuery('response) = {
      status: [ | `loading | `error(Js.Promise.error) | `success('response)],
      isFetching,
      isStale,
      failureCount,
      refetch,
      fetchMore,
      canFetchMore,
    };

    let convertInfinite:
      useInfiniteQuery_unmodified('response) => useInfiniteQuery('response) =
      result => {
        status:
          switch (
            result.status,
            result.data->Js.Nullable.toOption,
            result.error->Js.Nullable.toOption,
          ) {
          | ("success", Some(data), _) => `success(data)
          | ("error", _, Some(error)) => `error(error)
          | ("loading", _, _)
          | _ => `loading
          },
        isFetching: result.isFetching,
        isStale: result.isStale,
        failureCount: result.failureCount,
        refetch: result.refetch,
        fetchMore: result.fetchMore,
        canFetchMore: result.canFetchMore,
      };
  };

  [@bs.module "react-query"]
  external useQuery:
    (
      ~key: option(key('keyVars)),
      ~vars: 'optionalVars=?,
      ~fn: (string, 'keyVars, 'optionalVars) => Js.Promise.t('response),
      ~options: Options.query('queryFnParamsFilter, 'response)=?
    ) =>
    Result.useQuery_unmodified('response) =
    "useQuery";

  let useQuery =
      (
        ~key: option(key('keyVars)),
        ~vars: option('optionalVars)=?,
        ~options: option(Options.query('queryFnParamsFilter, 'response))=?,
        fn,
      ) =>
    useQuery(~key, ~vars?, ~fn, ~options?)->Result.convertQuery;

  [@bs.module "react-query"]
  external usePaginatedQuery:
    (
      ~key: option(key('keyVars)),
      ~vars: 'optionalVars=?,
      ~fn: (string, 'keyVars, 'optionalVars) => Js.Promise.t('response),
      ~options: Options.paginatedQuery('response)=?
    ) =>
    Result.usePaginatedQuery_unmodified('response) =
    "usePaginatedQuery";

  let usePaginatedQuery =
      (
        ~key: option(key('keyVars))=?,
        ~vars: option('optionalVars)=?,
        ~options: option(Options.paginatedQuery('response))=?,
        fn,
      ) =>
    usePaginatedQuery(~key, ~vars?, ~fn, ~options?)->Result.convertPaginated;

  [@bs.module "react-query"]
  external useInfiniteQuery:
    (
      ~key: option(key('keyVars)),
      ~vars: 'optionalVars=?,
      ~fn: (string, 'keyVars, 'optionalVars) => Js.Promise.t('response),
      ~options: Options.infiniteQuery('response)=?
    ) =>
    Result.useInfiniteQuery_unmodified('response) =
    "useInfiniteQuery";

  let useInfiniteQuery =
      (
        ~key: option(key('keyVars))=?,
        ~vars: option('optionalVars)=?,
        ~options: option(Options.infiniteQuery('response))=?,
        fn,
      ) =>
    useInfiniteQuery(~key, ~vars?, ~fn, ~options?)->Result.convertInfinite;

  [@bs.module "react-query"]
  external useMutation:
    (
      ~fn: 'vars => Js.Promise.t('response),
      ~options: Options.mutate('response, 'mutateVars, 'mutateData)=?
    ) =>
    Result.useMutation_unmodified('response, 'mutateVars, 'mutateData) =
    "useMutation";

  let useMutation =
      (
        ~options: option(Options.mutate('response, 'mutateVars, 'mutateData))=?,
        fn,
      ) =>
    useMutation(~options?, ~fn)->Result.convertMutation;
};

module ConfigProvider = {
  // https://github.com/tannerlinsley/react-query#reactqueryconfigprovider
  [@bs.deriving abstract]
  type config(
    'queryKey,
    'serializerResponse,
    'onMutate,
    'onSuccess,
    'onError,
    'onSettled,
    'queryFnParamsFilter,
    'prevData,
    'nextData,
  ) = {
    [@bs.optional]
    suspense: bool,
    [@bs.optional]
    useErrorBoundary: bool,
    [@bs.optional]
    throwOnError: bool,
    [@bs.optional]
    refetchAllOnWindowFocus: bool,
    [@bs.optional]
    queryKeySerializerFn: string => 'serializerResponse,
    [@bs.optional]
    onMutate: 'onMutate,
    [@bs.optional]
    onSuccess: 'onSuccess,
    [@bs.optional]
    onError: 'onError,
    [@bs.optional]
    onSettled: 'onSettled,
    [@bs.optional]
    retry: Hooks.Options.retry,
    [@bs.optional]
    retryDelay: Hooks.Options.retryDelay,
    [@bs.optional]
    staleTime: Hooks.Options.staleTime,
    [@bs.optional]
    cacheTime: Hooks.Options.cacheTime,
    [@bs.optional]
    refetchInterval: Hooks.Options.refetchInterval,
    [@bs.optional]
    queryFnParamsFilter:
      Hooks.Options.queryFnParamsFilter('queryFnParamsFilter),
    [@bs.optional]
    refetchOnMount: Hooks.Options.refetchOnMount,
    [@bs.optional]
    isDataEqual: ('prevData, 'nextData) => bool,
    [@bs.optional]
    refetchOnWindowFocus: Hooks.Options.refetchOnWindowFocus,
  };

  /**WARNING: config prop must be stable or memoized. Do not create an inline object!  */
  [@react.component] [@bs.module "react-query"]
  external make:
    (
      ~children: React.element,
      ~config: config(
                 (string, 'keyVars),
                 'serializerResponse,
                 'onMutate,
                 'onSuccess,
                 'onError,
                 'onSettled,
                 'queryFnParamsFilter,
                 'prevData,
                 'nextData,
               )
    ) =>
    React.element =
    "ReactQueryConfigProvider";
};

module Cache = {
  type t;
  type queryObject;
  type callback = t => unit;

  [@bs.module "react-query"] external cache: t = "queryCache";

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external clear: unit => unit = "clear";

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external subscribe: callback => unit = "subscribe";

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external isFetching: bool = "isFetching";

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external getQueries: key('keyVars) => array(queryObject) = "getQueries";

  [@bs.deriving abstract]
  type removeQueriesConfig = {
    [@bs.optional]
    exact: bool,
  };

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external removeQueries:
    (~key: key('keyVars), ~config: removeQueriesConfig=?, unit) => unit =
    "removeQueries";

  [@bs.deriving abstract]
  type cancelQueriesConfig = {
    [@bs.optional]
    exact: bool,
  };

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external cancelQueries:
    (~key: key('keyVars), ~config: cancelQueriesConfig=?, unit) => unit =
    "cancelQueries";

  [@bs.deriving abstract]
  type refetchQueriesConfig = {
    [@bs.optional]
    exact: bool,
    [@bs.optional]
    throwOnError: bool,
    [@bs.optional]
    force: bool,
  };

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external refetchQueries:
    (~key: key('keyVars), ~config: refetchQueriesConfig=?, unit) =>
    Js.Nullable.t('data) =
    "refetchQueries";

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external setQueryData:
    (~key: key('keyVars), ~updater: 'updater) => Js.Nullable.t('data) =
    "setQueryData";

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external getQueryData: key('keyVars) => Js.Nullable.t('data) =
    "getQueryData";

  [@bs.deriving abstract]
  type prefetchQueryConfig = {
    [@bs.optional]
    throwOnError: bool,
  };

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external prefetchQuery:
    (
      ~key: 'queryKey=?,
      ~vars: 'queryVariables=?,
      ~fn: 'queryFn=?,
      ~config: prefetchQueryConfig=?,
      unit
    ) =>
    Js.Promise.t('response) =
    "prefetchQuery";
};

module CacheProvider = {
  [@bs.module "react-query"]
  external makeQueryCache: unit => Cache.t = "makeQueryCache";

  [@react.component] [@bs.module "react-query"]
  external make:
    (~queryCache: Cache.t, ~children: React.element) => React.element =
    "ReactQueryCacheProvider";
};

module SetConsole = {
  [@bs.deriving abstract]
  type params('log, 'warn, 'error) = {
    log: 'log,
    warn: 'warn,
    error: 'error,
  };
  [@bs.module "react-query"]
  external make: unit => params('log, 'warn, 'error) = "setConsole";
};

[@bs.module "react-query"]
external useQueryCache: unit => Cache.t = "useQueryCache";

[@bs.module "react-query"]
external useIsFetching: unit => int = "useIsFetching";

module Helpers = {
  let queryIsSuccess = (query: Hooks.Result.useQuery('response)) =>
    switch (query.status) {
    | `success(_) => true
    | _ => false
    };

  let allQueriesAreSuccess =
      (queries: array(Hooks.Result.useQuery('response))) =>
    queries
    ->Array.keep(query =>
        switch (query.status) {
        | `success(_) => true
        | _ => false
        }
      )
    ->Array.length
    == queries->Array.length;

  let paginatedIsSuccess = (query: Hooks.Result.usePaginatedQuery('response)) =>
    switch (query.status) {
    | `success(_) => true
    | _ => false
    };

  let allPaginatedAreSuccess =
      (queries: array(Hooks.Result.usePaginatedQuery('response))) =>
    queries
    ->Array.keep(query =>
        switch (query.status) {
        | `success(_) => true
        | _ => false
        }
      )
    ->Array.length
    == queries->Array.length;

  let infiniteIsSuccess = (query: Hooks.Result.useInfiniteQuery('response)) =>
    switch (query.status) {
    | `success(_) => true
    | _ => false
    };

  let allInfiniteAreSuccess =
      (queries: array(Hooks.Result.useInfiniteQuery('response))) =>
    queries
    ->Array.keep(query =>
        switch (query.status) {
        | `success(_) => true
        | _ => false
        }
      )
    ->Array.length
    == queries->Array.length;
};