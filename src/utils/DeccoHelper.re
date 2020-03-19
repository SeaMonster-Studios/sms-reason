open Belt;

let encode = (x, ~tToJs) => tToJs(x) |> Decco.stringToJson;
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

module type PolymorphicVariantWithJsConverter = {
  type t;
  let tToJs: t => string;
  let tFromJs: Js.String.t => option(t);
  let name: string;
};

module MakePV = (PV: PolymorphicVariantWithJsConverter) => {
  include PV;

  let t_encode = encode(~tToJs);
  let t_decode = decode(~tFromJs, ~name);
  let t_decode_array = decode_array(~tFromJs, ~name);
};