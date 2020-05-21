module PropagateLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (~size: int=?, ~color: string=?, ~loading: bool, ~css: string=?) =>
    React.element =
    "PropagateLoader";
};

module BarLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (
      ~width: int=?,
      ~height: int=?,
      ~color: string=?,
      ~loading: bool,
      ~css: string=?
    ) =>
    React.element =
    "BarLoader";
};

module BeatLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (
      ~size: int=?,
      ~color: string=?,
      ~loading: bool,
      ~css: string=?,
      ~margin: int=?
    ) =>
    React.element =
    "BeatLoader";
};

module BounceLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (~size: int=?, ~color: string=?, ~loading: bool, ~css: string=?) =>
    React.element =
    "BounceLoader";
};

module CircleLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (~size: int=?, ~color: string=?, ~loading: bool, ~css: string=?) =>
    React.element =
    "CircleLoader";
};

module ClimbingBoxLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (~size: int=?, ~color: string=?, ~loading: bool, ~css: string=?) =>
    React.element =
    "ClimbingBoxLoader";
};

module ClipLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (~size: int=?, ~color: string=?, ~loading: bool, ~css: string=?) =>
    React.element =
    "ClipLoader";
};

module ClockLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (~size: int=?, ~color: string=?, ~loading: bool, ~css: string=?) =>
    React.element =
    "ClockLoader";
};

module DotLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (
      ~size: int=?,
      ~color: string=?,
      ~loading: bool,
      ~css: string=?,
      ~margin: int=?
    ) =>
    React.element =
    "DotLoader";
};

module FadeLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (
      ~height: int=?,
      ~width: int=?,
      ~radius: int=?,
      ~color: string=?,
      ~loading: bool,
      ~css: string=?,
      ~margin: int=?
    ) =>
    React.element =
    "FadeLoader";
};

module GridLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (~size: int=?, ~color: string=?, ~loading: bool, ~css: string=?) =>
    React.element =
    "GridLoader";
};

module HashLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (
      ~size: int=?,
      ~color: string=?,
      ~loading: bool,
      ~css: string=?,
      ~margin: int=?
    ) =>
    React.element =
    "HashLoader";
};

module MoonLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (
      ~size: int=?,
      ~color: string=?,
      ~loading: bool,
      ~css: string=?,
      ~margin: int=?
    ) =>
    React.element =
    "MoonLoader";
};

module PacmanLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (
      ~size: int=?,
      ~color: string=?,
      ~loading: bool,
      ~css: string=?,
      ~margin: int=?
    ) =>
    React.element =
    "PacmanLoader";
};

module PulseLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (
      ~size: int=?,
      ~color: string=?,
      ~loading: bool,
      ~css: string=?,
      ~margin: int=?
    ) =>
    React.element =
    "PulseLoader";
};

module RingLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (
      ~size: int=?,
      ~color: string=?,
      ~loading: bool,
      ~css: string=?,
      ~margin: int=?
    ) =>
    React.element =
    "RingLoader";
};

module RiseLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (
      ~size: int=?,
      ~color: string=?,
      ~loading: bool,
      ~css: string=?,
      ~margin: int=?
    ) =>
    React.element =
    "RiseLoader";
};

module RotateLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (
      ~size: int=?,
      ~color: string=?,
      ~loading: bool,
      ~css: string=?,
      ~margin: int=?
    ) =>
    React.element =
    "RotateLoader";
};

module SyncLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (
      ~size: int=?,
      ~color: string=?,
      ~loading: bool,
      ~css: string=?,
      ~margin: int=?
    ) =>
    React.element =
    "SyncLoader";
};

module ScaleLoader = {
  [@bs.module "react-spinners"] [@react.component]
  external make:
    (
      ~height: int=?,
      ~width: int=?,
      ~radius: int=?,
      ~color: string=?,
      ~loading: bool,
      ~css: string=?,
      ~margin: int=?
    ) =>
    React.element =
    "ScaleLoader";
};