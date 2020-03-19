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
    let make = (~children: React.element, ~index as _index: int) =>
      <> children </>;
  };

  module Hoc = {
    include Component;
    let make = withSortableElement(make);
  };
};

/**
 * EXAMPLE

[@react.component]
let make =
    (
      ~widgets: array(Widget.widget),
      ~onUpdate: Widget.widget => unit,
      ~onDelete: string => unit,
      ~onOrderChange: array(Widget.widget) => unit,
    ) => {
  <div className=Style.root>
    <Sms.ReactSortable.Container.Hoc
      onSortEnd={({oldIndex, newIndex}) => {
        let updatedWidgets = arrayMove(widgets, oldIndex, newIndex);
        onOrderChange(
          updatedWidgets->Array.mapWithIndex((index, widget) =>
            {...widget, displayIndex: index}
          ),
        );
      }}
      axis="xy"
      distance=5
      helperClass="is-dragging">
      <div>
        {widgets
         ->sortWidgets
         ->Array.mapWithIndex((index, widget) =>
             <Sms.ReactSortable.Item.Hoc index>
               <div
                 className={Style.widgetContainer(widgets->Array.length)}
                 key={index->string_of_int}>
                 <WidgetItem widget onUpdate onDelete />
               </div>
             </Sms.ReactSortable.Item.Hoc>
           )
         ->React.array}
      </div>
    </Sms.ReactSortable.Container.Hoc>
  </div>;
};
 */;