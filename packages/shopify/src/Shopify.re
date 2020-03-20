open Belt;

/**
 * Currently only setup for use with Bold's Recurring Orders
 * SETUP:
    * ALL: SET VARIABLES IN LIQUID (within <script>)
      * window.boldGroupId = "{{product.variants[0].metafields.bold_rp.rp_group_id}}"
    * BUNDLES: SET VARIABLES IN LIQUID (within <script>)
      {% assign is_bundle = false %}
      {% if product.metafields.sms.bundle_products != blank %}
        {% assign is_bundle = true %}
      {% endif %}
      window.is_bundle = window.is_bundle = {{ is_bundle | json }};
      window.products = []
      {% for product_handle in product.metafields.sms.bundle_products %}
        window.products = [...window.products, {{ all_products[product_handle] | json }}]
      {% endfor %}
    * SINGLE PRODUCT: SET VARIABLES IN LIQUID (within <script>)
      window.products = [{{ product | json }}]
    * OPTIONAL VARIABLES:
      window.custom_original_price = "{{ product.metafields.sms.custom_original_price }}"
 */

type orderType =
  | SingleOrder
  | Subscription;

type variant = {
  id: int,
  title: string,
  option1: Js.Nullable.t(string),
  option2: Js.Nullable.t(string),
  option3: Js.Nullable.t(string),
  name: string,
  public_title: string,
  options: array(string),
  price: int,
  compare_at_price: Js.Nullable.t(int),
  available: bool,
};

type product = {
  title: string,
  handle: string,
  id: int,
  price: int,
  price_varies: bool,
  price_min: int,
  price_max: int,
  available: bool,
  compare_at_price: Js.Nullable.t(int),
  compare_at_price_min: int,
  compare_at_price_max: int,
  compare_at_price_varies: bool,
  options: array(string),
  variants: array(variant),
};

type selectItem = {
  id: string,
  inputLabel: string,
  productIndex: int,
  optionIndex: int,
};

type selectError = {
  productIndex: int,
  message: string,
};

let blankSelectItem = {
  id: "",
  inputLabel: "",
  productIndex: 0,
  optionIndex: 0,
};

type selectedProductOptions = array(array(selectItem));

[@decco]
type cartItemSubscriptionProperties = {
  group_id: int,
  frequency_num: int,
  frequency_type: int,
  frequency_type_text: string,
  _ro_discount_percentage: int,
};

[@decco]
type cartItemSubscription = {
  id: int,
  quantity: int,
  properties: cartItemSubscriptionProperties,
};

[@decco]
type cartItemsSubscription = {items: array(cartItemSubscription)};

[@decco]
type cartItem('a) = {
  id: int,
  quantity: int,
};

type cartItems('a) = {items: array(cartItem('a))};

[@bs.val] [@bs.scope "window"]
external products_: Js.Nullable.t(array(product)) = "products";

[@bs.val] [@bs.scope "window"]
external customOriginalPrice_: Js.Nullable.t(string) =
  "custom_original_price";

let customOriginalPrice =
  switch (customOriginalPrice_->Js.Nullable.toOption) {
  | None => None
  | Some(original_price) => Some(original_price->float_of_string)
  };

let defaultProductFormError = "There was an unknown problem adding your items to the cart. Please try again later.";
let dataError = "There was a problem retrieving this product's data. Please try again later.";

module Encode = {
  external cartItemsToJson: cartItems('a) => Js.Json.t = "%identity";
  // let cartItemSubscriptionProperties = (c: cartItemSubscriptionProperties) =>
  //   Json.Encode.(
  //     object_([
  //       ("group_id", c.group_id->int),
  //       ("frequency_num", c.frequency_num->int),
  //       ("frequency_type", c.frequency_type->int),
  //       ("frequency_type_text", c.frequency_type_text->string),
  //       ("_ro_discount_percentage", c._ro_discount_percentage->int),
  //     ])
  //   );
  // let cartItemSubscription = (c: cartItemSubscription) =>
  //   Json.Encode.(
  //     object_([
  //       ("id", c.id->int),
  //       ("quantity", c.quantity->int),
  //       ("properties", c.properties->cartItemSubscriptionProperties),
  //       ("bold-ro__selector_radio_button", "2"->string),
  //     ])
  //   );
  // let cartItemSubscriptions = Json.Encode.array(cartItemSubscription);
  // let cartItemsSubscription = (c: cartItemsSubscription) =>
  //   Json.Encode.(object_([("items", c.items->cartItemSubscriptions)]));
};

module Bold = {
  type frequencyType = {
    frequency_type_translation: string,
    frequency_type_id: int,
  };

  type groupInfo = {
    discount_percentage: float,
    group_discount: int,
    group_id: int,
    s_shop_url: string,
    shop_url: string,
    app_root: string,
    money_format: string,
    frequency_num: int,
    /**This is the max frequency */
    frequency_type: array(frequencyType),
  };

  let blankGroupInfo = {
    discount_percentage: 0.,
    group_discount: 0,
    group_id: 0,
    s_shop_url: "",
    shop_url: "",
    app_root: "",
    money_format: "",
    frequency_num: 0,
    frequency_type: [||],
  };

  type frequency = {
    max: int,
    intervalLabel: string,
    intervalId: int,
  };

  let blankFrequency = {max: 0, intervalLabel: "", intervalId: 0};

  let getFrequency = groupInfo => {
    switch (groupInfo.frequency_type[0]) {
    | Some(item) =>
      Some({
        max: groupInfo.frequency_num,
        intervalId: item.frequency_type_id,
        intervalLabel: item.frequency_type_translation,
      })
    | None => None
    };
  };

  [@bs.module "@boldcommerce/bold-subscriptions-js"]
  [@bs.scope ("widgetAPI", "subscriptionGroups")]
  external getGroupInfo_: string => Js.Promise.t(groupInfo) = "getGroupInfo";

  [@bs.val] [@bs.scope "window"]
  external isBundle: Js.Nullable.t(bool) = "is_bundle";

  [@bs.val] [@bs.scope "window"]
  external groupId_: Js.Nullable.t(string) = "boldGroupId";
  let groupId = groupId_->Js.Nullable.toOption->Option.getWithDefault("");
  let getGroupInfo = () => getGroupInfo_(groupId);
};

module Utils = {
  let redirectToCartPage: unit => unit = [%raw
    {|
  function() {
    window.location.href = "/cart"
  }
|}
  ];

  let getOptionValueFromVariant: (variant, int) => string = [%raw
    {|
  function(variant, optionIndex) {
    const key = "option" + (optionIndex + 1)
    return variant[key]
  }
|}
  ];

  let getInitialSelectedProductOptions = (products: array(product)) =>
    products->Array.mapWithIndex((productIndex, product) =>
      product.options
      ->Array.mapWithIndex((optionIndex, _option) => {
          // First check to see if there is a variant that is available, if there is use that and if not use the first variant
          switch (product.variants->Array.getBy(variant => variant.available)) {
          | Some(availableVariant) =>
            let value =
              availableVariant->getOptionValueFromVariant(optionIndex);
            {id: value, inputLabel: value, productIndex, optionIndex};
          | None =>
            switch (product.variants[0]) {
            | Some(notAvailableVariant) =>
              let value =
                notAvailableVariant->getOptionValueFromVariant(optionIndex);
              {id: value, inputLabel: value, productIndex, optionIndex};
            | None => blankSelectItem
            }
          }
        })
    );

  let getPriceLabel = (price: float) =>
    "$" ++ (price /. 100.)->Js.Float.toFixedWithPrecision(~digits=2);

  let getDiscountedPrice = (price: float, discount: float) =>
    price *. discount;

  let getFrequencySelectOptions = (frequency: Bold.frequency) => {
    Array.make(frequency.max, () => None)
    ->Array.mapWithIndex((index, _) =>
        {
          id: (index + 1)->string_of_int,
          inputLabel:
            (index + 1)->string_of_int
            ++ " "
            ++ frequency.intervalLabel
            ++ (index == 0 ? "" : "s"),
          productIndex: 0,
          optionIndex: 0,
        }
      );
  };

  let getProductOptionsSelectValues: (product, int) => array(string) = [%raw
    {|
  function(product, optionIndex) {
    const key = "option" + (optionIndex + 1)
    return product.variants.reduce((acc, variant) => {
      return acc.includes(variant[key]) ? acc : [...acc, variant[key]]
    }, [])
  }
|}
  ];

  let getProductOptionSelectValue =
      (selectedOptions: selectedProductOptions, productIndex, optionIndex) =>
    switch (selectedOptions[productIndex]) {
    | Some(selectedProductOptions) =>
      switch (selectedProductOptions[optionIndex]) {
      | Some(value) => value
      | None => blankSelectItem
      }
    | None => blankSelectItem
    };

  let handleUpdateSelectedValues =
      (selectedOptions: selectedProductOptions, value: selectItem) => {
    selectedOptions->Array.mapWithIndex(
      (productIndex, selectedProductOptions) =>
      selectedProductOptions->Array.mapWithIndex((optionIndex, option) =>
        value.productIndex == productIndex && value.optionIndex == optionIndex
          ? value : option
      )
    );
  };

  let getProductVariantFromSelections =
      (
        selectedOptions: selectedProductOptions,
        product: product,
        productIndex,
      ) => {
    switch (selectedOptions[productIndex]) {
    | Some(selectedProductOptions) =>
      product.variants
      ->Array.getBy(variant =>
          selectedProductOptions->Array.every(item =>
            variant->getOptionValueFromVariant(item.optionIndex) == item.id
          )
        )
    | None => None
    };
  };

  let getPriceOfProducts = (products: array(product)) =>
    products->Array.reduce(0., (acc, product) => {
      switch (product.variants[0]) {
      | None => acc +. product.price->float_of_int
      | Some(variant) => acc +. variant.price->float_of_int
      }
    });

  let getComparePriceOfProducts = (products: array(product)) =>
    products->Array.reduce(0., (acc, product) =>
      switch (product.variants[0]) {
      | None =>
        switch (product.compare_at_price->Js.Nullable.toOption) {
        | Some(compare_at_price) => acc +. compare_at_price->float_of_int
        | None => acc +. product.price->float_of_int
        }
      | Some(variant) =>
        switch (variant.compare_at_price->Js.Nullable.toOption) {
        | Some(compare_at_price) => acc +. compare_at_price->float_of_int
        | None => acc +. variant.price->float_of_int
        }
      }
    );

  let getPriceOfProductsBySelections =
      (selectedOptions: selectedProductOptions, products: array(product)) =>
    products->Array.reduceWithIndex(0., (acc, product, productIndex) => {
      switch (
        selectedOptions->getProductVariantFromSelections(
          product,
          productIndex,
        )
      ) {
      | Some(variant) => acc +. variant.price->float_of_int
      | None =>
        switch (product.variants[0]) {
        | Some(variant) => acc +. variant.price->float_of_int
        | None => acc +. product.price->float_of_int
        }
      }
    });

  let getComparePriceOfProductsBySelections =
      (selectedOptions: selectedProductOptions, products: array(product)) =>
    products->Array.reduceWithIndex(0., (acc, product, productIndex) => {
      switch (
        selectedOptions->getProductVariantFromSelections(
          product,
          productIndex,
        )
      ) {
      | Some(variant) =>
        switch (variant.compare_at_price->Js.Nullable.toOption) {
        | Some(compare_at_price) => acc +. compare_at_price->float_of_int
        | None =>
          switch (product.compare_at_price->Js.Nullable.toOption) {
          | Some(compare_at_price) => acc +. compare_at_price->float_of_int
          | None => acc +. variant.price->float_of_int
          }
        }
      | None => acc +. product.price->float_of_int
      }
    });

  let getSelectedOptionErrors = (selectedOptions, productsWithOptions) =>
    productsWithOptions->Array.reduceWithIndex(
      [||],
      (acc, product, productIndex) => {
        let newError = {
          productIndex,
          message: "This combination of options for this product isn't currently available.",
        };
        switch (
          selectedOptions->getProductVariantFromSelections(
            product,
            productIndex,
          )
        ) {
        | Some(variant) =>
          variant.available ? acc : acc->Array.concat([|newError|])
        | None => acc->Array.concat([|newError|])
        };
      },
    );

  let getPrices =
      (productsWithoutOptions, productsWithOptions, selectedOptions) => {
    let price =
      productsWithoutOptions->getPriceOfProducts
      +. selectedOptions->getPriceOfProductsBySelections(productsWithOptions);

    let comparePrice =
      productsWithoutOptions->getComparePriceOfProducts
      +. selectedOptions->getComparePriceOfProductsBySelections(
           productsWithOptions,
         );

    (price, comparePrice);
  };
};

type fetch('s, 'f) =
  | Loading
  | NotAsked
  | Success('s)
  | Failure('f);

type useSubscriptionFormValue = {
  productsWithOptions: array(product),
  handleOptionChange: selectItem => unit,
  getOptionValue: (~productIndex: int, ~optionIndex: int) => selectItem,
  getOptionSelectItems:
    (~productIndex: int, ~optionIndex: int, ~product: product) =>
    array(selectItem),
  getOptionError: (~productIndex: int) => option(string),
  isSubscription: bool,
  setIsSubscription: unit => unit,
  setIsNotSubscription: unit => unit,
  subscriptionPriceLabel: string,
  subscriptionComparePriceLabel: string,
  priceLabel: string,
  comparePriceLabel: string,
  price: float,
  comparePrice: float,
  customOriginalPrice: option(float),
  customOriginalPriceLabel: option(string),
  handleSubscriptionFrequencyChange: selectItem => unit,
  deliveryFrequency: selectItem,
  subscriptionDeliveryFrequencyItems: array(selectItem),
  quantity: int,
  handleQuantityUpdate: int => unit,
  formSubmitStatus: fetch(string, string),
  disableSubmit: bool,
  status: fetch(unit, string),
  handleSubmit: (~redirectToCartOnSuccess: bool=?, unit) => unit,
};

type state = {
  products: array(product),
  productsWithOptions: array(product),
  productsWithoutOptions: array(product),
  status: fetch(unit, string),
  formSubmitStatus: fetch(string, string),
  groupInfo: Bold.groupInfo,
  frequency: Bold.frequency,
  selectedOptions: array(array(selectItem)),
  selectedOptionsErrors: array(selectError),
  price: float,
  comparePrice: float,
  orderType,
  quantity: int,
  deliveryFrequency: selectItem,
};

type action =
  | SetStatus(fetch(unit, string))
  | SetBoldGroupInfo(Bold.groupInfo, Bold.frequency)
  | SetSelectedOptions(array(array(selectItem)))
  | SetDeliveryFrequency(selectItem)
  | SetOrderType(orderType)
  | SetQuantity(int)
  | SetFormSubmitStatus(fetch(string, string));

let reducer = (state, action) => {
  switch (action) {
  | SetFormSubmitStatus(formSubmitStatus) => {...state, formSubmitStatus}
  | SetDeliveryFrequency(deliveryFrequency) => {...state, deliveryFrequency}
  | SetQuantity(quantity) => {...state, quantity}
  | SetStatus(status) => {...state, status}
  | SetOrderType(orderType) => {
      ...state,
      orderType,
      quantity: orderType == Subscription ? 1 : state.quantity,
    }
  | SetBoldGroupInfo(groupInfo, frequency) => {
      ...state,
      groupInfo,
      frequency,
      status: Success(),
      deliveryFrequency: {
        id: "1",
        inputLabel: "1 " ++ frequency.intervalLabel,
        productIndex: 0,
        optionIndex: 0,
      },
    }
  | SetSelectedOptions(selectedOptions) =>
    let (price, comparePrice) =
      Utils.getPrices(
        state.productsWithoutOptions,
        state.productsWithOptions,
        selectedOptions,
      );
    {
      ...state,
      price,
      comparePrice,
      selectedOptions,
      selectedOptionsErrors:
        selectedOptions->Utils.getSelectedOptionErrors(
          state.productsWithOptions,
        ),
    };
  };
};

module Hooks = {
  let useSubscriptionForm = () => {
    let (state, dispatch) =
      React.useReducer(
        reducer,
        {
          let products =
            switch (products_->Js.Nullable.toOption) {
            | Some(products) => products
            | None =>
              Js.log(
                "window.products was not found. Please refer to `Shopify.re` for instructions on how to use the useSubscriptionForm hook.",
              );
              [||];
            };

          let productsWithoutOptions =
            switch (Bold.isBundle->Js.Nullable.toOption) {
            | Some(true) =>
              products->Array.keep(product =>
                product.options->Array.length <= 1
              )
            | _ => [||]
            };

          let productsWithOptions =
            switch (Bold.isBundle->Js.Nullable.toOption) {
            | Some(true) =>
              products->Array.keep(product =>
                product.options->Array.length > 1
              )
            | _ => products
            };

          let selectedOptions =
            productsWithOptions->Utils.getInitialSelectedProductOptions;

          let selectedOptionsErrors =
            selectedOptions->Utils.getSelectedOptionErrors(
              productsWithOptions,
            );

          let (price, comparePrice) =
            Utils.getPrices(
              productsWithoutOptions,
              productsWithOptions,
              selectedOptions,
            );

          {
            quantity: 1,
            orderType: SingleOrder,
            formSubmitStatus: NotAsked,
            price,
            comparePrice,
            products,
            productsWithOptions,
            productsWithoutOptions,
            selectedOptions,
            selectedOptionsErrors,
            status: Loading,
            groupInfo: Bold.blankGroupInfo,
            frequency: Bold.blankFrequency,
            deliveryFrequency: blankSelectItem,
          };
        },
      );

    React.useEffect1(
      () => {
        Js.Promise.(
          Bold.getGroupInfo()
          |> then_(response => {
               switch (response->Bold.getFrequency) {
               | Some(frequency) =>
                 dispatch(SetBoldGroupInfo(response, frequency))
               | None => dispatch(SetStatus(Failure(dataError)))
               };
               resolve();
             })
          |> catch(error => {
               dispatch(SetStatus(Failure(dataError)));
               Js.log(error);
               resolve();
             })
        )
        ->ignore;
        None;
      },
      [|dispatch|],
    );

    let disableSubmit = state.selectedOptionsErrors->Array.length > 0;

    let handleSubmit = (~redirectToCartOnSuccess=false, ()) => {
      dispatch(SetFormSubmitStatus(Loading));
      Js.Promise.(
        (
          switch (state.orderType) {
          | Subscription =>
            let properties = {
              group_id: state.groupInfo.group_id,
              frequency_num: state.deliveryFrequency.id->int_of_string,
              frequency_type: state.frequency.intervalId,
              frequency_type_text: state.frequency.intervalLabel ++ "(s)",
              _ro_discount_percentage: state.groupInfo.group_discount,
            };
            let items =
              state.productsWithOptions
              ->Array.mapWithIndex((productIndex, product) =>
                  switch (
                    state.selectedOptions
                    ->Utils.getProductVariantFromSelections(
                        product,
                        productIndex,
                      )
                  ) {
                  | Some(variant) =>
                    let item: cartItemSubscription = {
                      id: variant.id,
                      quantity: state.quantity,
                      properties,
                    };
                    item;
                  | None => {
                      id: product.id,
                      quantity: state.quantity,
                      properties,
                    }
                  }
                )
              ->Array.concat(
                  state.productsWithoutOptions
                  ->Array.map(product =>
                      switch (product.variants[0]) {
                      | Some(variant) => {
                          id: variant.id,
                          quantity: state.quantity,
                          properties,
                        }
                      | None => {
                          id: product.id,
                          quantity: state.quantity,
                          properties,
                        }
                      }
                    ),
                );
            if (state.selectedOptionsErrors->Array.length == 0) {
              Fetch.fetchWithInit(
                "/cart/add.js",
                Fetch.RequestInit.make(
                  ~method_=Post,
                  ~body=
                    Fetch.BodyInit.make(
                      Js.Json.stringify(
                        {items: items}->cartItemsSubscription_encode,
                      ),
                    ),
                  ~headers=
                    Fetch.HeadersInit.make({
                      "Content-Type": "application/json",
                    }),
                  (),
                ),
              );
            } else {
              dispatch(
                SetFormSubmitStatus(Failure(defaultProductFormError)),
              );
              Js.Promise.make((~resolve as _res, ~reject as _rej) => ());
            };
          | SingleOrder =>
            let items =
              state.productsWithOptions
              ->Array.mapWithIndex((productIndex, product) =>
                  switch (
                    state.selectedOptions
                    ->Utils.getProductVariantFromSelections(
                        product,
                        productIndex,
                      )
                  ) {
                  | Some(variant) => {
                      id: variant.id,
                      quantity: state.quantity,
                    }
                  | None => {id: product.id, quantity: state.quantity}
                  }
                )
              ->Array.concat(
                  state.productsWithoutOptions
                  ->Array.map(product =>
                      switch (product.variants[0]) {
                      | Some(variant) => {
                          id: variant.id,
                          quantity: state.quantity,
                        }
                      | None => {id: product.id, quantity: state.quantity}
                      }
                    ),
                );
            if (state.selectedOptionsErrors->Array.length == 0) {
              Fetch.fetchWithInit(
                "/cart/add.js",
                Fetch.RequestInit.make(
                  ~method_=Post,
                  ~body=
                    Fetch.BodyInit.make(
                      Js.Json.stringify(
                        {items: items}->Encode.cartItemsToJson,
                      ),
                    ),
                  ~headers=
                    Fetch.HeadersInit.make({
                      "Content-Type": "application/json",
                    }),
                  (),
                ),
              );
            } else {
              dispatch(
                SetFormSubmitStatus(Failure(defaultProductFormError)),
              );
              Js.Promise.make((~resolve as _res, ~reject as _rej) => ());
            };
          }
        )
        |> then_(_response => {
             if (disableSubmit == true) {
               dispatch(
                 SetFormSubmitStatus(
                   Failure("Please resolve the errors above"),
                 ),
               );
             } else {
               dispatch(
                 SetFormSubmitStatus(
                   Success(
                     "The product was successfully added to your cart!",
                   ),
                 ),
               );
               if (redirectToCartOnSuccess) {
                 Utils.redirectToCartPage();
               };
             };

             resolve();
           })
        |> catch(error => {
             Js.log(error);
             dispatch(
               SetFormSubmitStatus(Failure(defaultProductFormError)),
             );
             resolve();
           })
      )
      ->ignore;
    };

    {
      productsWithOptions: state.productsWithOptions,
      handleOptionChange: value =>
        dispatch(
          SetSelectedOptions(
            state.selectedOptions->Utils.handleUpdateSelectedValues(value),
          ),
        ),
      getOptionValue: (~productIndex, ~optionIndex) =>
        state.selectedOptions
        ->Utils.getProductOptionSelectValue(productIndex, optionIndex),
      getOptionSelectItems: (~productIndex, ~optionIndex, ~product) =>
        product
        ->Utils.getProductOptionsSelectValues(optionIndex)
        ->Array.map(option => {
            {id: option, inputLabel: option, productIndex, optionIndex}
          }),
      getOptionError: (~productIndex) =>
        switch (
          state.selectedOptionsErrors
          ->Array.getBy(item => item.productIndex == productIndex)
        ) {
        | Some(error) => Some(error.message)
        | None => None
        },
      isSubscription: state.orderType == Subscription,
      setIsSubscription: () => dispatch(SetOrderType(Subscription)),
      setIsNotSubscription: () => dispatch(SetOrderType(SingleOrder)),
      subscriptionPriceLabel:
        state.price
        ->Utils.getDiscountedPrice(state.groupInfo.discount_percentage)
        ->Utils.getPriceLabel,
      subscriptionComparePriceLabel:
        state.comparePrice
        ->Utils.getDiscountedPrice(state.groupInfo.discount_percentage)
        ->Utils.getPriceLabel,
      priceLabel:
        (state.price *. state.quantity->float_of_int)->Utils.getPriceLabel,
      comparePriceLabel:
        (state.comparePrice *. state.quantity->float_of_int)
        ->Utils.getPriceLabel,
      price: state.price,
      comparePrice: state.comparePrice,
      handleSubscriptionFrequencyChange: value =>
        dispatch(SetDeliveryFrequency(value)),
      deliveryFrequency: state.deliveryFrequency,
      subscriptionDeliveryFrequencyItems:
        state.frequency->Utils.getFrequencySelectOptions,
      quantity: state.quantity,
      handleQuantityUpdate: quantity => dispatch(SetQuantity(quantity)),
      formSubmitStatus: state.formSubmitStatus,
      handleSubmit,
      disableSubmit,
      status: state.status,
      customOriginalPrice:
        switch (customOriginalPrice) {
        | None => None
        | Some(originalPrice) =>
          Some(originalPrice *. state.quantity->float_of_int)
        },
      customOriginalPriceLabel:
        switch (customOriginalPrice) {
        | None => None
        | Some(originalPrice) =>
          Some(
            (originalPrice *. state.quantity->float_of_int)
            ->Utils.getPriceLabel,
          )
        },
    };
  };
};