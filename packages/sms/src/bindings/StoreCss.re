type cssPrams = {
  url: string,
  storage: string,
  crossOrigin: string,
};

[@bs.module "store-css"] external make: cssPrams => unit = "css";