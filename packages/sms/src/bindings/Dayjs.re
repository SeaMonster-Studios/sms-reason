type dayjs;

[@bs.module] external dayjs: string => dayjs = "dayjs";

[@bs.send] external format: (dayjs, string) => string = "format";