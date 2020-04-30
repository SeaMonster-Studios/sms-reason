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
          ~lockAxis as _lockAxis: option(string)=?,
          ~distance as _disatance: int,
          ~helperClass as _helperClass: string,
          ~onSortEnd as _onSortEnd: sortEndParams => unit,
          ~useDragHandle as _useDragHandle: bool=false,
        ) =>
      <> children </>;
  };

  include Component;
  let make = withSortableContainer(make);
};

module Item = {
  [@bs.module "react-sortable-hoc"]
  external withSortableElement: Hoc.t('a) = "sortableElement";

  module Component = {
    [@react.component]
    let make = (~children: React.element, ~index as _index: int) =>
      <> children </>;
  };

  include Component;
  let make = withSortableElement(make);
};

module Handle = {
  [@bs.module "react-sortable-hoc"]
  external withSortableHandle: Hoc.t('a) = "sortableHandle";

  module Component = {
    [@react.component]
    let make = (~children: React.element) => <> children </>;
  };

  include Component;
  let make = withSortableHandle(make);
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
    <SeamonsterStudiosReason.ReactSortable.Container
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
             <SeamonsterStudiosReason.ReactSortable.Item index key={index->string_of_int}>
               <div
                 className={Style.widgetContainer(widgets->Array.length)}>
                 <WidgetItem widget onUpdate onDelete />
               </div>
             </SeamonsterStudiosReason.ReactSortable.Item>
           )
         ->React.array}
      </div>
    </SeamonsterStudiosReason.ReactSortable.Container>
  </div>;
};
 */;