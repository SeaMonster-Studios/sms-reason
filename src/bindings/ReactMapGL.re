type width = string;
type height = string;
type latitude = float;
type longitude = float;
type zoom = int;

type viewport = {
  .
  "width": width,
  "height": height,
  "latitude": latitude,
  "longitude": longitude,
  "zoom": zoom,
};

type mapOptions = {. "trackResize": bool};

[@bs.module "react-map-gl"] [@react.component]
external make:
  (
    ~width: width,
    ~height: height,
    ~latitude: latitude,
    ~longitude: longitude,
    ~zoom: zoom,
    ~mapboxApiAccessToken: string,
    ~mapOptions: mapOptions=?,
    ~mapStyle: string=?,
    ~scrollZoom: bool=?,
    ~onViewportChange: viewport => unit=?,
    ~dragPan: bool=?,
    ~dragRotate: bool=?,
    ~children: React.element=?
  ) =>
  React.element =
  "default";

module SVGOverlay = {
  type redraw = {
    .
    "project": array(int) => array(int),
    "width": width,
    "height": height,
    "latitude": latitude,
    "longitude": longitude,
    "zoom": zoom,
  };

  [@bs.module "react-map-gl"] [@react.component]
  external make: (~redraw: redraw => React.element) => React.element =
    "SVGOverlay";
};

module Marker = {
  type event = {. "lngLat": array(int)};

  [@bs.module "react-map-gl"] [@react.component]
  external make:
    (
      ~latitude: latitude,
      ~longitude: longitude,
      ~offsetLeft: int=?,
      ~offsetTop: int=?,
      ~draggable: bool=?,
      ~onDragStart: unit => unit=?,
      ~onDrag: unit => unit=?,
      ~onDragEnd: unit => unit=?,
      ~captureScroll: bool=?,
      ~captureDrag: bool=?,
      ~captureClick: bool=?,
      ~captureDoubleClick: bool=?,
      ~className: string=?,
      ~children: React.element
    ) =>
    React.element =
    "Marker";
};

module NavigationControl = {
  [@bs.module "react-map-gl"] [@react.component]
  external make:
    (
      ~onViewportChange: unit => unit=?,
      ~showCompass: bool=?,
      ~showZoom: bool=?,
      ~captureScroll: bool=?,
      ~captureClick: bool=?,
      ~captureDrag: bool=?,
      ~captureDoubleClick: bool=?
    ) =>
    React.element =
    "NavigationControl";
};

module Popup = {
  [@bs.module "react-map-gl"] [@react.component]
  external make:
    (
      ~latitude: latitude,
      ~longitude: longitude,
      ~altitude: int=?,
      ~offsetLeft: int=?,
      ~offsetTop: int=?,
      ~closeButton: bool=?,
      ~closeOnClick: bool=?,
      ~tipSize: int=?,
      ~anchor: [@bs.string] [
                 | `bottom
                 | `top
                 | `right
                 | `left
                 | [@bs.as "top-left"] `topLeft
                 | [@bs.as "top-right"] `topRight
                 | [@bs.as "bottom-left"] `bottomLeft
                 | [@bs.as "bottom-right"] `bottomRight
               ]
                 =?,
      ~dynamicPosition: bool=?,
      ~sortByDepth: bool=?,
      ~onClose: unit => unit=?,
      ~captureScroll: bool=?,
      ~captureClick: bool=?,
      ~captureDrag: bool=?,
      ~captureDoubleClick: bool=?,
      ~className: string=?,
      ~children: React.element
    ) =>
    React.element =
    "Popup";
};