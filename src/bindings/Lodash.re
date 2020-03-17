[@bs.module] external startCase: string => string = "lodash.startcase";
[@bs.module] external unique: array('a) => array('a) = "lodash.uniq";
[@bs.module]
external sortby: (array('a), array('a => 'b)) => array('a) =
  "lodash.sortby";
[@bs.module]
external difference: (array('a), array('a)) => array('a) =
  "lodash.difference";
[@bs.module] external isEqual: ('a, 'a) => bool = "lodash.isequal";