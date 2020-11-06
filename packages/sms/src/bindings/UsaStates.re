open Belt;

type state = {
  name: string,
  abbreviation: string,
  territory: bool,
  capital: string,
  contiguous: bool,
};

type t = {states: array(state)};

[@bs.module "usa-states"] [@bs.new]
external getStates: unit => t = "UsaStates";

let states = getStates().states->Array.map(state => state.name);
