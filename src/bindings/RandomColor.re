type params = {
  count: int,
  format: string,
  luminosity: string,
};

[@bs.module "randomcolor"] external make: params => array(string) = "default";