open ReasonDateFns;

[@bs.val] [@bs.scope "localStorage"]
external removeItem: string => unit = "removeItem";

[@bs.val] [@bs.scope "localStorage"]
external getItem: string => Js.Nullable.t(string) = "getItem";

[@bs.val] [@bs.scope "localStorage"]
external setItem: (string, string) => unit = "setItem";

let getItemAndDecode = (key, decoder) =>
  switch (getItem(key)->Js.Nullable.toOption) {
  | Some(cachedState) =>
    switch (cachedState->Js.Json.parseExn->decoder) {
    | Ok(decoded) => Some(decoded)
    | Error(error) =>
      error->SentryBrowser.captureDeccoError(
        "Local Storage Item with key: " ++ key,
      );
      None;
    }
  | None => None
  };

[@decco]
type itemWithExpiration('a) = {
  expiresAt: float,
  data: 'a,
};

type itemResponse('a) =
  | Cache('a)
  | Expired
  | NoCache;

let getItemWithExpiration = (storage_key, decoder) =>
  switch (getItem(storage_key)->Js.Nullable.toOption) {
  | None => NoCache
  | Some(jsonString) =>
    switch (
      jsonString->Js.Json.parseExn |> itemWithExpiration_decode(decoder)
    ) {
    | Ok(cache) =>
      if (DateFns.isBefore(
            Js.Date.fromFloat(cache.expiresAt),
            Js.Date.make(),
          )) {
        Cache(cache.data);
      } else {
        Expired;
      }
    | Error(error) =>
      error->SentryBrowser.captureDeccoError(
        "getItemWithExpiration, key: " ++ storage_key,
      );
      Expired;
    }
  };

let setItemWithExpiration = (storage_key, data, encoder) =>
  {expiresAt: DateFns.addDays(1, Js.Date.make())->Js.Date.getTime, data}
  |> itemWithExpiration_encode(encoder)
  |> Js.Json.stringify
  |> setItem(storage_key);
