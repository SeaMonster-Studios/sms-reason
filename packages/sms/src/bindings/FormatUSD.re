type formatUSDProps = {
  amount: float,
  decimalPlaces: int,
};

[@bs.module] external make: formatUSDProps => float = "format-usd";

let format0 = amount => make({amount, decimalPlaces: 0});