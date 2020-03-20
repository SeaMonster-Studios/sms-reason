%raw
{|
  /*eslint-disable no-unused-expressions */
  |};
/** The eslint disable is almost necessary b/c you'll sometimes need to pass props that are used by the HOC but not the component that is wrapped to the HOC */

type t('p) = React.component('p) => React.component('p);