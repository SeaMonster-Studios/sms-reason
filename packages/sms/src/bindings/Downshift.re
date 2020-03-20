/** Developed with "downshift": "^3.4.1" */

type highlightedIndex = int;
type isOpen = bool;

[@bs.module "downshift"] [@bs.scope "useSelect"]
external stateChangeTypes: {
  .
  "MenuKeyDownArrowDown": string,
  "MenuKeyDownArrowUp": string,
  "MenuKeyDownEscape": string,
  "MenuKeyDownHome": string,
  "MenuKeyDownEnd": string,
  "MenuKeyDownEnter": string,
  "MenuKeyDownCharacter": string,
  "MenuBlur": string,
  "ItemHover": string,
  "ItemClick": string,
  "ToggleButtonKeyDownCharacter": string,
  "ToggleButtonKeyDownArrowDown": string,
  "ToggleButtonKeyDownArrowUp": string,
  "ToggleButtonClick": string,
  "FunctionToggleMenu": string,
  "FunctionOpenMenu": string,
  "FunctionCloseMenu": string,
  "FunctionSetHighlightedIndex": string,
  "FunctionSelectItem": string,
  "FunctionClearKeysSoFar": string,
  "FunctionReset": string,
} =
  "stateChangeTypes";

type toggleButtonProps = {
  .
  "aria-expanded": bool,
  "aria-haspopup": string,
  "aria-labelledby": string,
  "id": string,
  [@bs.meth] "onClick": ReactEvent.Mouse.t => unit,
  [@bs.meth] "onKeyDown": ReactEvent.Keyboard.t => unit,
  "ref": ReactDOMRe.domRef,
};

type labelProps = {
  .
  "id": string,
  "htmlFor": string,
};

type menuProps = {
  .
  "aria-labelledby": string,
  "id": string,
  [@bs.meth] "onBlur": ReactEvent.Focus.t => unit,
  [@bs.meth] "onKeyDown": ReactEvent.Keyboard.t => unit,
  [@bs.meth] "onMouseLeave": ReactEvent.Mouse.t => unit,
  "ref": ReactDOMRe.domRef,
  "role": string,
  "tabIndex": int,
};

type itemProps = {
  .
  "role": string,
  "id": string,
  [@bs.meth] "onClick": ReactEvent.Mouse.t => unit,
  [@bs.meth] "onMouseMove": ReactEvent.Mouse.t => unit,
  "ref": ReactDOMRe.domRef,
};

[@bs.deriving abstract]
type createGetItemPropsParams('i) = {
  [@bs.optional]
  index: highlightedIndex,
  [@bs.optional]
  item: 'i,
};

type state('i) = {
  .
  "highlightedIndex": highlightedIndex,
  "isOpen": bool,
  "keysSoFar": string,
  "selectedItem": Js.Nullable.t('i),
};

type actionAndChanges('i) = {
  .
  "changes": state('i),
  "type": string,
  "props": {
    .
    "items": array('i),
    "initialSelectedItem": 'i,
    [@bs.meth] "itemToString": 'i => string,
    [@bs.meth] "getA11ySelectionMessage": unit => string,
    [@bs.meth] "getA11yStatusMessage": unit => string,
    [@bs.meth]
    "scrollIntoView": (ReactDOMRe.domRef, ReactDOMRe.domRef) => unit,
    [@bs.meth] "stateReducer": (state('i), state('i)) => state('i),
  },
};

[@bs.deriving abstract]
type createUseSelectParams('i) = {
  items: array('i),
  [@bs.optional]
  initialSelectedItem: 'i,
  [@bs.optional]
  itemToString: 'i => string,
  [@bs.optional]
  onSelectedItemChange: state('i) => unit,
  [@bs.optional]
  stateReducer: (state('i), actionAndChanges('i)) => state('i),
  [@bs.optional]
  initialIsOpen: isOpen,
  [@bs.optional]
  initialHighlightedIndex: highlightedIndex,
  [@bs.optional]
  defaultSelectedItem: 'i,
  [@bs.optional]
  defaultIsOpen: isOpen,
  [@bs.optional]
  defaultHighlightedIndex: highlightedIndex,
  [@bs.optional]
  getA11yStatusMessage: unit => string,
  [@bs.optional]
  getA11ySelectionMessage: unit => string,
  [@bs.optional]
  onHighlightedIndexChange: state('i) => unit,
  [@bs.optional]
  onIsOpenChange: state('i) => unit,
  [@bs.optional]
  onStateChange: state('i) => unit,
  [@bs.optional]
  highlightedIndex,
  [@bs.optional]
  selectedItem: 'i,
  [@bs.optional]
  id: string,
  [@bs.optional]
  labelId: string,
  [@bs.optional]
  menuId: string,
  [@bs.optional]
  toggleButtonId: string,
  [@bs.optional]
  getItemId: string,
  [@bs.optional]
  circularNavigation: bool,
};

type useSelectValue('i) = {
  .
  "isOpen": isOpen,
  "selectedItem": Js.Nullable.t('i),
  "highlightedIndex": highlightedIndex,
  [@bs.meth] "getToggleButtonProps": unit => toggleButtonProps,
  [@bs.meth] "getLabelProps": unit => labelProps,
  [@bs.meth] "getMenuProps": unit => menuProps,
  /**Usage: getItemProps(createGetItemPropsParams(~index=0, ())) */
  [@bs.meth]
  "getItemProps": createGetItemPropsParams('i) => itemProps,
  "toggleMenu": unit => unit,
  "selectItem": 'i => unit,
  "openMenu": unit => unit,
  "closeMenu": unit => unit,
  "reset": unit => unit,
  [@bs.meth] "setHighlightedIndex": highlightedIndex => unit,
};

[@bs.module "downshift"]
external useSelect: createUseSelectParams('i) => useSelectValue('i) =
  "useSelect";

/**Usage: useSelect(createUseSelectParams(~items=options, ())); */;