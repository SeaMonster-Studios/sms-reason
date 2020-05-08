[@bs.module "email-validator"] external email: string => bool = "validate";
[@bs.module "url-validator"] external url: string => bool = "default";

module PostalCode = {
  [@bs.module "postcode-validator"]
  external validatorExists: string => bool = "postcodeValidatorExists";
  [@bs.module "postcode-validator"]
  external validator: (string, string) => bool = "postcodeValidator";

  let us = code => validator(code, "US");
};