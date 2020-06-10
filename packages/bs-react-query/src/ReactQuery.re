[%bs.raw {|
  /* eslint-disable react-hooks/rules-of-hooks */
  ""
|}];

type cache;

module type OptionTypes = {type data;};

module OptionTypes = (Config: OptionTypes) => {
  type data;
  type manual = bool;
  type retry = int;
  type retryDelay = int => int;
  type staleTime = int;
  type cacheTime = int;
  type refetchInterval = int;
  type refetchIntervalInBackground = bool;
  type refetchOnWindowFocus = bool;
  type suspense = bool;
  type initialData = Config.data;
  type refetchOnMount = bool;
  type getFetchMore = (Config.data, array(Config.data)) => bool;
};

module type ResultTypes = {type data;};

module ResultTypes = (Config: ResultTypes) => {
  type status_ = string;
  type error_ = Js.Nullable.t(Js.Promise.error);
  type data_ = Js.Nullable.t(Config.data);
  type isFetching = bool;
  type isStale = bool;
  type failureCount = int;
  type refetch = unit => unit;
  type fetchMore = unit => unit;
  type canFetchMore = bool;
};

module Cache = {
  /** These are global cache elements. All other Cache properties and methods have types specific to a query and so exist within each query module */
  type callback = cache => unit;
  [@bs.module "react-query"] external cache: cache = "queryCache";

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external clear: unit => unit = "clear";

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external subscribe: callback => unit = "subscribe";

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external isFetching: bool = "isFetching";
};

module type MakeCache = {
  type key;
  type data;
  type vars;
};

module MakeCache = (Config: MakeCache) => {
  type queryFn = (Config.key, Config.vars) => Js.Promise.t(Config.data);

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external get: Config.key => array(Config.data) = "getQueries";

  [@bs.deriving abstract]
  type removeConfig = {
    [@bs.optional]
    exact: bool,
  };

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external remove: (~key: Config.key, ~config: removeConfig=?, unit) => unit =
    "removeQueries";

  [@bs.deriving abstract]
  type cancelConfig = {
    [@bs.optional]
    exact: bool,
  };

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external cancel: (~key: Config.key, ~config: cancelConfig=?, unit) => unit =
    "cancelQueries";

  [@bs.deriving abstract]
  type refetchConfig = {
    [@bs.optional]
    exact: bool,
    [@bs.optional]
    throwOnError: bool,
    [@bs.optional]
    force: bool,
  };

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external refetch:
    (~key: Config.key, ~config: refetchConfig=?, unit) =>
    Js.Nullable.t(Config.data) =
    "refetchQueries";

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external setData:
    (Config.key, Js.Nullable.t(Config.data) => Js.Nullable.t(Config.data)) =>
    unit =
    "setQueryData";

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external getData: Config.key => Js.Nullable.t(Config.data) = "getQueryData";

  [@bs.deriving abstract]
  type prefetchConfig = {
    [@bs.optional]
    throwOnError: bool,
  };

  [@bs.module "react-query"] [@bs.scope "queryCache"]
  external prefetch:
    (
      ~key: Config.key=?,
      ~vars: Config.vars=?,
      ~fn: queryFn=?,
      ~config: prefetchConfig=?,
      unit
    ) =>
    Js.Promise.t(Config.data) =
    "prefetchQuery";
};

module type Query = {
  type key;
  type vars;
  type data;
};

module Query = (Config: Query) => {
  type queryFn = (Config.key, Config.vars) => Js.Promise.t(Config.data);

  module Cache =
    MakeCache({
      type data = Config.data;
      type key = Config.key;
      type vars = Config.vars;
    });

  module Options = {
    include OptionTypes({
      type data = Config.data;
    });

    [@bs.deriving abstract]
    type make = {
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
      onSuccess: Config.data => unit,
      [@bs.optional]
      onError: Js.Promise.error => unit,
      [@bs.optional]
      onSettled: (Config.data, Js.Promise.error) => unit,
      [@bs.optional]
      suspense,
      [@bs.optional]
      initialData,
      [@bs.optional]
      refetchOnMount,
    };
  };

  module Result = {
    include ResultTypes({
      type data = Config.data;
    });

    type t_unmodified = {
      status: status_,
      error: error_,
      data: data_,
      isFetching,
      isStale,
      failureCount,
      refetch,
    };

    type status =
      | Loading
      | Error(Js.Promise.error)
      | Success(Config.data);

    type t = {
      status,
      isFetching,
      isStale,
      failureCount,
      refetch,
    };

    let isSuccess = t =>
      switch (t.status) {
      | Success(_) => true
      | _ => false
      };

    let isError = t =>
      switch (t.status) {
      | Error(_) => true
      | _ => false
      };

    let isLoading = t =>
      switch (t.status) {
      | Loading => true
      | _ => false
      };

    let makeReasonable: t_unmodified => t =
      query => {
        status:
          switch (
            query.status,
            query.data->Js.Nullable.toOption,
            query.error->Js.Nullable.toOption,
          ) {
          | ("success", Some(data), _) => Success(data)
          | ("error", _, Some(error)) => Error(error)
          | ("loading", _, _)
          | _ => Loading
          },
        isFetching: query.isFetching,
        isStale: query.isStale,
        failureCount: query.failureCount,
        refetch: query.refetch,
      };

    let useMakeReasonable: t_unmodified => t =
      query => {
        let prevQuery = SeamonsterStudiosReactHooks.Hooks.usePrevious(query);
        let (reasonable, setReasonable) =
          React.useState(() => query->makeReasonable);

        React.useEffect3(
          () => {
            if (!LodashIsEqual.make(query, prevQuery)) {
              setReasonable(_ => query->makeReasonable);
            };
            None;
          },
          (makeReasonable, query, prevQuery),
        );

        reasonable;
      };
  };

  [@bs.module "react-query"]
  external use:
    (
      ~key: option(Config.key),
      ~vars: Config.vars=?,
      ~fn: queryFn,
      ~options: Options.make=?
    ) =>
    Result.t_unmodified =
    "useQuery";

  let use =
      (
        ~key: option(Config.key),
        ~vars: option(Config.vars)=?,
        ~options: option(Options.make)=?,
        fn,
      ) =>
    use(~key, ~vars?, ~fn, ~options?)->Result.useMakeReasonable;
};

module Paginated = (Config: Query) => {
  type queryFn = (Config.key, Config.vars) => Js.Promise.t(Config.data);

  module Cache =
    MakeCache({
      type data = Config.data;
      type key = Config.key;
      type vars = Config.vars;
    });

  module Options = {
    include OptionTypes({
      type data = Config.data;
    });

    [@bs.deriving abstract]
    type make = {
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
      onSuccess: Config.data => unit,
      [@bs.optional]
      onError: Js.Promise.error => unit,
      [@bs.optional]
      suspense,
      [@bs.optional]
      initialData,
      [@bs.optional]
      refetchOnMount,
    };
  };

  module Result = {
    include ResultTypes({
      type data = Config.data;
    });

    type t_unmodified = {
      status: status_,
      error: error_,
      resolvedData: data_,
      latestData: data_,
      isFetching,
      isStale,
      failureCount,
      refetch,
    };

    type status =
      | Loading
      | Error(Js.Promise.error)
      | Success(Config.data, Config.data);

    type t = {
      status,
      isFetching,
      isStale,
      failureCount,
      refetch,
    };

    let isSuccess = t =>
      switch (t.status) {
      | Success(_) => true
      | _ => false
      };

    let isError = t =>
      switch (t.status) {
      | Error(_) => true
      | _ => false
      };

    let isLoading = t =>
      switch (t.status) {
      | Loading => true
      | _ => false
      };

    let makeReasonable: t_unmodified => t =
      query => {
        status:
          switch (
            query.status,
            query.resolvedData->Js.Nullable.toOption,
            query.latestData->Js.Nullable.toOption,
            query.error->Js.Nullable.toOption,
          ) {
          | ("success", Some(resolvedData), Some(latestData), _) =>
            Success(resolvedData, latestData)
          | ("error", _, _, Some(error)) => Error(error)
          | ("loading", _, _, _)
          | _ => Loading
          },
        isFetching: query.isFetching,
        isStale: query.isStale,
        failureCount: query.failureCount,
        refetch: query.refetch,
      };

    let useMakeReasonable: t_unmodified => t =
      query => {
        let prevQuery = SeamonsterStudiosReactHooks.Hooks.usePrevious(query);
        let (reasonable, setReasonable) =
          React.useState(() => query->makeReasonable);

        React.useEffect3(
          () => {
            if (!LodashIsEqual.make(query, prevQuery)) {
              setReasonable(_ => query->makeReasonable);
            };
            None;
          },
          (makeReasonable, query, prevQuery),
        );

        reasonable;
      };
  };

  [@bs.module "react-query"]
  external use:
    (
      ~key: option(Config.key),
      ~vars: Config.vars=?,
      ~fn: queryFn,
      ~options: Options.make=?
    ) =>
    Result.t_unmodified =
    "usePaginatedQuery";

  let use =
      (
        ~key: option(Config.key),
        ~vars: option(Config.vars)=?,
        ~options: option(Options.make)=?,
        fn,
      ) =>
    use(~key, ~vars?, ~fn, ~options?)->Result.useMakeReasonable;
};

module Infinite = (Config: Query) => {
  type queryFn = (Config.key, Config.vars) => Js.Promise.t(Config.data);

  module Cache =
    MakeCache({
      type data = Config.data;
      type key = Config.key;
      type vars = Config.vars;
    });

  module Options = {
    include OptionTypes({
      type data = Config.data;
    });

    [@bs.deriving abstract]
    type make = {
      [@bs.optional]
      getFetchMore,
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
      onSuccess: Config.data => unit,
      [@bs.optional]
      onError: Js.Promise.error => unit,
      [@bs.optional]
      suspense,
      [@bs.optional]
      initialData,
      [@bs.optional]
      refetchOnMount,
    };
  };

  module Result = {
    include ResultTypes({
      type data = Config.data;
    });

    type t_unmodified = {
      status: status_,
      error: error_,
      data: data_,
      isFetching,
      isStale,
      failureCount,
      refetch,
      fetchMore,
      canFetchMore,
    };

    type status =
      | Loading
      | Error(Js.Promise.error)
      | Success(Config.data);

    type t = {
      status,
      isFetching,
      isStale,
      failureCount,
      refetch,
      fetchMore,
      canFetchMore,
    };

    let isSuccess = t =>
      switch (t.status) {
      | Success(_) => true
      | _ => false
      };

    let isError = t =>
      switch (t.status) {
      | Error(_) => true
      | _ => false
      };

    let isLoading = t =>
      switch (t.status) {
      | Loading => true
      | _ => false
      };

    let makeReasonable: t_unmodified => t =
      query => {
        status:
          switch (
            query.status,
            query.data->Js.Nullable.toOption,
            query.error->Js.Nullable.toOption,
          ) {
          | ("success", Some(data), _) => Success(data)
          | ("error", _, Some(error)) => Error(error)
          | ("loading", _, _)
          | _ => Loading
          },
        isFetching: query.isFetching,
        isStale: query.isStale,
        failureCount: query.failureCount,
        refetch: query.refetch,
        fetchMore: query.fetchMore,
        canFetchMore: query.canFetchMore,
      };

    let useMakeReasonable: t_unmodified => t =
      query => {
        let prevQuery = SeamonsterStudiosReactHooks.Hooks.usePrevious(query);
        let (reasonable, setReasonable) =
          React.useState(() => query->makeReasonable);

        React.useEffect3(
          () => {
            if (!LodashIsEqual.make(query, prevQuery)) {
              setReasonable(_ => query->makeReasonable);
            };
            None;
          },
          (makeReasonable, query, prevQuery),
        );

        reasonable;
      };
  };

  [@bs.module "react-query"]
  external use:
    (
      ~key: option(Config.key),
      ~vars: Config.vars=?,
      ~fn: queryFn,
      ~options: Options.make=?
    ) =>
    Result.t_unmodified =
    "useInfiniteQuery";

  let use =
      (
        ~key: option(Config.key),
        ~vars: option(Config.vars)=?,
        ~options: option(Options.make)=?,
        fn,
      ) =>
    use(~key, ~vars?, ~fn, ~options?)->Result.useMakeReasonable;
};

module type Mutation = {
  type vars;
  type data;
};

module Mutation = (Config: Mutation) => {
  type mutationFn = Config.vars => Js.Promise.t(Config.data);

  module Options = {
    include OptionTypes({
      type data = Config.data;
    });

    type onMutate = Config.vars => Js.Promise.t(Config.data);

    type onSuccess =
      (Config.data, Config.vars) => Js.Nullable.t(Js.Promise.t(unit));

    type onError =
      (Js.Promise.error, Config.vars, Config.data) =>
      Js.Nullable.t(Js.Promise.t(unit));

    type onSettled =
      (Config.data, Js.Promise.error, Config.vars, Config.data) =>
      Js.Nullable.t(Js.Promise.t(unit));

    [@bs.deriving abstract]
    type make = {
      [@bs.optional]
      onMutate,
      [@bs.optional]
      onSuccess,
      [@bs.optional]
      onError,
      [@bs.optional]
      onSettled,
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
      initialData,
      [@bs.optional]
      refetchOnMount,
    };
  };

  module Result = {
    include ResultTypes({
      type data = Config.data;
    });

    type mutate = Config.vars => Js.Promise.t(Config.data);

    type promise = Js.Promise.t(Config.data);

    type t_unmodified_rec = {
      status: status_,
      error: error_,
      data: data_,
      promise,
      reset: unit => unit,
    };

    type t_unmodified = (mutate, t_unmodified_rec);

    type status =
      | Idle
      | Loading
      | Error(Js.Promise.error)
      | Success(Config.data);

    type t_rec = {
      status,
      promise,
      reset: unit => unit,
    };

    type t = (mutate, t_rec);

    let isSuccess = t_rec =>
      switch (t_rec.status) {
      | Success(_) => true
      | _ => false
      };

    let isError = t_rec =>
      switch (t_rec.status) {
      | Error(_) => true
      | _ => false
      };

    let isIdle = t_rec =>
      switch (t_rec.status) {
      | Idle => true
      | _ => false
      };

    let isLoading = t_rec =>
      switch (t_rec.status) {
      | Loading => true
      | _ => false
      };

    let makeReasonable: t_unmodified => t =
      ((mutate, result)) => (
        mutate,
        {
          status:
            switch (
              result.status,
              result.data->Js.Nullable.toOption,
              result.error->Js.Nullable.toOption,
            ) {
            | ("success", Some(data), _) => Success(data)
            | ("error", _, Some(error)) => Error(error)
            | ("loading", _, _) => Loading
            | ("idle", _, _)
            | _ => Idle
            },
          promise: result.promise,
          reset: result.reset,
        },
      );

    let useMakeReasonable: t_unmodified => t =
      query => {
        let prevQuery = SeamonsterStudiosReactHooks.Hooks.usePrevious(query);
        let (reasonable, setReasonable) =
          React.useState(() => query->makeReasonable);

        React.useEffect3(
          () => {
            if (!LodashIsEqual.make(query, prevQuery)) {
              setReasonable(_ => query->makeReasonable);
            };
            None;
          },
          (makeReasonable, query, prevQuery),
        );

        reasonable;
      };
  };

  [@bs.module "react-query"]
  external use:
    (
      ~fn: Config.vars => Js.Promise.t(Config.data),
      ~options: Options.make=?
    ) =>
    Result.t_unmodified =
    "useMutation";

  let use = (~options: option(Options.make)=?, fn) =>
    use(~fn, ~options?)->Result.useMakeReasonable;
};

module ConfigProvider = {
  // https://github.com/tannerlinsley/react-query#reactqueryconfigprovider
  [@bs.deriving abstract]
  type config(
    'serializerParam,
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
    queryKeySerializerFn: 'serializerParam => 'serializerResponse,
    [@bs.optional]
    onMutate: 'onMutate,
    [@bs.optional]
    onSuccess: 'onSuccess,
    [@bs.optional]
    onError: 'onError,
    [@bs.optional]
    onSettled: 'onSettled,
    [@bs.optional]
    retry: int,
    [@bs.optional]
    retryDelay: int => int,
    [@bs.optional]
    staleTime: int,
    [@bs.optional]
    cacheTime: int,
    [@bs.optional]
    refetchInterval: int,
    [@bs.optional]
    refetchOnMount: bool,
    [@bs.optional]
    isDataEqual: ('prevData, 'nextData) => bool,
    [@bs.optional]
    refetchOnWindowFocus: bool,
  };

  /**WARNING: config prop must be stable or memoized. Do not create an inline object!  */
  [@react.component] [@bs.module "react-query"]
  external make:
    (
      ~children: React.element,
      ~config: config(
                 'serializerParam,
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

module CacheProvider = {
  [@bs.module "react-query"]
  external makeQueryCache: unit => cache = "makeQueryCache";

  [@react.component] [@bs.module "react-query"]
  external make:
    (~queryCache: cache, ~children: React.element) => React.element =
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
external useQueryCache: unit => cache = "useQueryCache";

[@bs.module "react-query"]
external useIsFetching: unit => int = "useIsFetching";

module type Set = {
  type key;
  type data;
  type queryVars;
  type mutateVars;
};

module QuerySet = (Config: Set) => {
  module Query =
    Query({
      type key = Config.key;
      type data = Config.data;
      type vars = Config.queryVars;
    });

  module Mutation =
    Mutation({
      type data = Config.data;
      type vars = Config.mutateVars;
    });
};

module PaginatedSet = (Config: Set) => {
  module Paginated =
    Paginated({
      type key = Config.key;
      type data = Config.data;
      type vars = Config.queryVars;
    });

  module Mutation =
    Mutation({
      type data = Config.data;
      type vars = Config.mutateVars;
    });
};

module InfiniteSet = (Config: Set) => {
  module Infinite =
    Infinite({
      type key = Config.key;
      type data = Config.data;
      type vars = Config.queryVars;
    });

  module Mutation =
    Mutation({
      type data = Config.data;
      type vars = Config.mutateVars;
    });
};

module Make = (Config: Set) => {
  module Query =
    Query({
      type key = Config.key;
      type data = Config.data;
      type vars = Config.queryVars;
    });

  module Paginated =
    Paginated({
      type key = Config.key;
      type data = Config.data;
      type vars = Config.queryVars;
    });

  module Infinite =
    Infinite({
      type key = Config.key;
      type data = Config.data;
      type vars = Config.queryVars;
    });

  module Mutation =
    Mutation({
      type data = Config.data;
      type vars = Config.mutateVars;
    });
};