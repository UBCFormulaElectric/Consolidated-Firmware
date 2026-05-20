export type CommonSignalMetadata = {
  name: string;
  tx_node: string;
  msg_name: string;
  id: number;
};

export enum SignalType {
  ENUM = "enum",
  NUMERICAL = "numerical",
  ALERT = "alert",
  BOOLEAN = "boolean"
}

export type EnumSignalMetadata = CommonSignalMetadata & {
  type: SignalType.ENUM;
  enum_signal: {
    enum_name: string,
    enum_values: { [value: string]: number },
  };
  min_val: number;
  max_val: number;
  cycle_time_ms: number | null;
};

export type NumericalSignalMetadata = CommonSignalMetadata & {
  type: SignalType.NUMERICAL;
  min_val: number;
  max_val: number;
  unit: "" | string;
  cycle_time_ms: number | null;
};

export type AlertSignalMetadata = CommonSignalMetadata & {
  type: SignalType.ALERT;
  min_val: 0;
  max_val: 1;
  cycle_time_ms: number | null;
};

export type BooleanSignalMetadata = CommonSignalMetadata & {
  type: SignalType.BOOLEAN;
  min_val: 0;
  max_val: 1;
  cycle_time_ms: number | null;
}

export const isBooleanSignalMetadata = (signal: SignalMetadata): signal is BooleanSignalMetadata => {
  return signal.type === SignalType.BOOLEAN;
}

export const isEnumSignalMetadata = (signal: SignalMetadata): signal is EnumSignalMetadata => {
  return signal.type === SignalType.ENUM;
};

export const isAlertSignalMetadata = (signal: SignalMetadata): signal is AlertSignalMetadata => {
  return signal.type === SignalType.ALERT;
};

export const isNumericalSignalMetadata = (signal: SignalMetadata): signal is NumericalSignalMetadata => {
  return signal.type === SignalType.NUMERICAL;
};

export type SignalMetadata = EnumSignalMetadata | NumericalSignalMetadata | AlertSignalMetadata | BooleanSignalMetadata;
export type WidgetSignalMetadata =
  | (BooleanSignalMetadata | EnumSignalMetadata)[]
  | NumericalSignalMetadata[]
  | AlertSignalMetadata[];
