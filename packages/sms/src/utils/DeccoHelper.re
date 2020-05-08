open Belt;

module PolymorphicVariantHelpers = {
  external t_to_string: 'a => string = "%identity";

  let encode = (x, ~tToJs) =>
    /** for some reason tToJs if failing with `x` is already a string */
    Js.typeof(x)
    == "string"
      ? x->t_to_string->Decco.stringToJson : x->tToJs->Decco.stringToJson;

  let decode = (x, ~tFromJs, ~name) =>
    Decco.stringFromJson(x)
    ->Result.map(tFromJs)
    ->Result.flatMap(y =>
        switch (y) {
        | Some(value) => Ok(value)
        | None => Decco.error("Unknown " ++ name, x)
        }
      );

  let decode_array = (x, ~tFromJs, ~name) => {
    x
    |> Decco.arrayFromJson(item =>
         Decco.stringFromJson(item)
         ->Result.map(tFromJs)
         ->Result.flatMap(y =>
             switch (y) {
             | Some(value) => Ok(value)
             | None => Decco.error("Unknown " ++ name, item)
             }
           )
       );
  };
};

// Make Polymorphic Variant
module type MakePV = {
  type t;
  let tToJs: t => string;
  let tFromJs: Js.String.t => option(t);
  let name: string;
};
module MakePV = (Config: MakePV) => {
  include Config;

  let t_encode = PolymorphicVariantHelpers.encode(~tToJs);
  let t_decode = PolymorphicVariantHelpers.decode(~tFromJs, ~name);
  let t_decode_array =
    PolymorphicVariantHelpers.decode_array(~tFromJs, ~name);

  /** Example
   module County =
    SeamonsterStudiosReason.DeccoHelper.MakePV({
      [@bs.deriving jsConverter]
      type t = [
        | `staunton
        | `salem
        | `bristol
        | `richmond
        | `culpeper
        | `fredrickburg
        | `lynchburg
        | `northern_virginia
        | `hampton_roads
      ];
      let name = "ConnectionStatus";
    });
   */;
};

// Make Polymorphic Variant with Value
module type MakePVV = {
  type t;
  let name: string;
  let encodePattern: (t, (string, option(Js.Json.t)) => 'a) => 'a;
  let decodePattern:
    (string, Js.Json.t, Decco.decodeError) =>
    Belt.Result.t(t, Decco.decodeError);
};
module MakePVV = (Config: MakePVV) => {
  include Config;

  let%private encoder: Decco.encoder(t) =
    t => {
      /** It's possible that one variant will have a value while another will not */
      let encode = (tag: string, value: option(Js.Json.t)): Js.Json.t =>
        {
          "tag": tag,
          "value": value->Belt.Option.getWithDefault(""->Decco.stringToJson),
        }
        ->Obj.magic;

      t->Config.encodePattern(encode);
    };

  let%private decoder: Decco.decoder(t) =
    json =>
      /** It's possible that one variant will have a value while another will not. If it doesn't then the value will be null. */
      (
        if (json->Js.typeof == "string") {
          json
          ->Decco.stringFromJson
          ->Belt.Result.flatMap(tag =>
              Config.decodePattern(
                tag,
                ""->Decco.stringToJson,
                {
                  path: {j|[$name]|j},
                  message: {j|$tag variant type does not exist on $name|j},
                  value: tag->Decco.stringToJson,
                }: Decco.decodeError,
              )
            );
        } else {
          switch (json->Js.Json.decodeObject) {
          | Some(dict) =>
            switch (dict->Js.Dict.get("tag"), dict->Js.Dict.get("value")) {
            | (Some(tag), Some(value)) =>
              tag
              ->Decco.stringFromJson
              ->Belt.Result.flatMap(tag =>
                  Config.decodePattern(
                    tag,
                    value,
                    {
                      path: {j|[$name].tag|j},
                      message: {j|$tag variant type does not exist on $name|j},
                      value: tag->Decco.stringToJson,
                    }: Decco.decodeError,
                  )
                )
            | (None, None) =>
              Error(
                {
                  path: {j|[$name].tag, [$name].value|j},
                  message: "variant fields `tag` and `value` do not exist",
                  value: json,
                }: Decco.decodeError,
              )
            | (Some(_), None) =>
              Error(
                {
                  path: {j|[$name].value|j},
                  message: "variant field `value` does not exist",
                  value: json,
                }: Decco.decodeError,
              )
            | (None, Some(_)) =>
              Error(
                {
                  path: {j|[$name].tag|j},
                  message: "variant field `tag` does not exist",
                  value: json,
                }: Decco.decodeError,
              )
            }
          | None =>
            Error(
              {path: "", message: "Not a dict", value: json}: Decco.decodeError,
            )
          };
        }
      );

  [@genType]
  [@decco]
  type t_ = [@decco.codec (encoder, decoder)] Config.t;
  let t_decode = t__decode;
  let t_encode = t__encode;

  /** Example
      [@decco]
      type someRecord = {
        stuff: string,
        other: int,
      };

      module Test =
        SeamonsterStudiosReason.DeccoHelper.MakePVV({
          let name = "Test Module";

          [@genType]
          type t = [ | `Account(string) | `NoAccount(someRecord)];

          let encodePattern = (t: t, encode) =>
            switch (t) {
            | `Account(string) => "Account"->encode(string->Decco.stringToJson)
            | `NoAccount(someRecord) =>
              "NoAccount"->encode(someRecord->someRecord_encode)
            };

          let decodePattern = (tag, value, decodeError) => {
            switch (tag) {
            | "Account" =>
              value->Decco.stringFromJson->Belt.Result.map(value => `Account(value))
            | "NoAccount" =>
              value->someRecord_decode->Belt.Result.map(value => `NoAccount(value))
            | _ => deccoError->Error
            };
          };
        });

      let account = `Account("a string");
      let no_account = `NoAccount({stuff: "stuff", other: 8});

      let accountE = account->Test.t_encode;
      let no_accountE = no_account->Test.t_encode;

      let accountD = accountE->Test.t_decode;
      let no_accountD = no_accountE->Test.t_decode;
     */;
};