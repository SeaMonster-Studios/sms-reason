type sortEndParams = {
  oldIndex: int,
  newIndex: int,
};

module Container = {
  [@bs.module "react-sortable-hoc"]
  external withSortableContainer: Hoc.t('a) = "sortableContainer";

  module Component = {
    [@react.component]
    let make =
        (
          ~children: React.element,
          ~axis as _axis: string,
          ~distance as _disatance: int,
          ~helperClass as _helperClass: string,
          ~onSortEnd as _onSortEnd: sortEndParams => unit,
        ) =>
      <> children </>;
  };

  module Hoc = {
    include Component;
    let make = withSortableContainer(make);
  };
};

module Item = {
  [@bs.module "react-sortable-hoc"]
  external withSortableElement: Hoc.t('a) = "sortableElement";

  module Component = {
    [@react.component]
    let make = (~children: React.element) => <> children </>;
  };

  module Hoc = {
    include Component;
    let make = withSortableElement(make);
  };
};