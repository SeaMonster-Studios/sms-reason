type viewport = ReactMapGL.viewport;

type datum = {
  coordinates: array(float),
  /**The following fields are custom. They're based on our nodes data */
  node_type: string,
  region: string,
  node_status: string,
  id: string,
};

type data = array(datum);

type icon = {
  url: string,
  width: int,
  height: int,
  anchorY: int,
};

type layer;

type clickInfo = {
  [@bs.as "object"]
  object_: option(datum),
  x: option(int),
  y: option(int),
};

type iconLayerParams('a) = {
  data,
  id: string,
  iconAtlas: string,
  iconMapping: Js.t('a),
  getIcon: datum => string,
  getSize: datum => float,
  pickable: bool,
  sizeScale: float,
  getPosition: datum => array(float),
  getColor: datum => array(int),
  onClick: clickInfo => unit,
};

type geoJsonLayerParams('a) = {
  data: Js.t('a),
  id: string,
  getFillColor: array(int),
  getLineColor: 'a => array(int),
  getLineWidth: int,
  stroked: bool,
  filled: bool,
  extruded: bool,
  lineWidthScale: int,
  lineWidthMinPixels: int,
  /*
   pickable: bool,
   getRadius: int,
   getElevation: int,
    onClick: clickInfo => unit,
    onHover: clickInfo => unit,
    */
};

[@bs.module "deck.gl"] [@bs.new]
external iconLayer: iconLayerParams('a) => layer = "IconLayer";

[@bs.module "deck.gl"] [@bs.new]
external geoJsonLayer: geoJsonLayerParams('a) => layer = "GeoJsonLayer";

[@bs.module "deck.gl"] [@react.component]
external make:
  (
    ~viewState: viewport,
    ~layers: array(layer),
    ~controller: bool=?,
    ~initialViewState: viewport=?,
    ~children: React.element=?
  ) =>
  React.element =
  "default";