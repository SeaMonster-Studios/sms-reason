type formatUSDProps = {
  amount: float,
  decimalPlaces: int,
};

[@bs.module] external make: formatUSDProps => string = "format-usd";

let format0 = amount => make({amount, decimalPlaces: 0});
let format1 = amount => make({amount, decimalPlaces: 1});
let format2 = amount => make({amount, decimalPlaces: 2});

