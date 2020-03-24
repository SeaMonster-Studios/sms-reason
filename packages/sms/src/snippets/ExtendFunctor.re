module F = (X: {}) => {
  let x = () => ();
};

module Extend = (F: (X: {}) => {let x: unit => unit;}, X: {}) => {
  include F(X);
  let u = x();
};

module G = Extend(F);