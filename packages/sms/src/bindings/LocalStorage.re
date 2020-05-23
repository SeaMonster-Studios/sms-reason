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
      error->Sentry.Browser.captureDeccoError(
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

let getItemWithExpiration = (storage_key, decoder) =>
  switch (getItem(storage_key)->Js.Nullable.toOption) {
  | None => None
  | Some(jsonString) =>
    switch (
      jsonString->Js.Json.parseExn |> itemWithExpiration_decode(decoder)
    ) {
    | Ok(cache) =>
      if (DateFns.isBefore(
            Js.Date.fromFloat(cache.expiresAt),
            Js.Date.make(),
          )) {
        Some(Ok(cache.data));
      } else {
        None;
      }
    | Error(error) => Some(Error(error))
    }
  };

let setItemWithExpiration = (storage_key, data, encoder) =>
  {expiresAt: DateFns.addDays(1., Js.Date.make())->Js.Date.getTime, data}
  |> itemWithExpiration_encode(encoder)
  |> Js.Json.stringify
  |> setItem(storage_key);