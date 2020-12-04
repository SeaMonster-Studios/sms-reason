type dayjs;

[@bs.module] external dayjs: string => dayjs = "dayjs";

[@bs.send] external format: (dayjs, string) => string = "format";

[@bs.send]
external diff:
  (
    dayjs,
    dayjs,
    [@bs.string] [
      | `millisecond
      | `second
      | `minute
      | `hour
      | `day
      | `week
      | `quarter
      | `month
      | `year
    ],
    bool
  ) =>
  float =
  "diff";
