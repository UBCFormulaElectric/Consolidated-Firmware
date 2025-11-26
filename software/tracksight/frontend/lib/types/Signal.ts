type CommonSignalMetadata = {
  name: string;
  tx_node: string;
  msg_name: string;
  id: number;
};

type EnumSignalMetadata = CommonSignalMetadata & {
  enum: {
    items: Record<number, string>;
    name: string;
  };
  min_val: number;
  max_val: number;
  cycle_time_ms: number | null;
};

type NumericalSignalMetadata = CommonSignalMetadata & {
  min_val: number;
  max_val: number;
  unit: "" | string;
  cycle_time_ms: number | null;
};

type AlertSignalMetadata = CommonSignalMetadata & {
  min_val: 0;
  max_val: 1;
  cycle_time_ms: number | null;
};

const isEnumSignalMetadata = (signal: SignalMetadata): signal is EnumSignalMetadata => {
  return (signal as EnumSignalMetadata).enum !== null;
};

const isAlertSignalMetadata = (signal: SignalMetadata): signal is AlertSignalMetadata => {
  return (
    (signal as AlertSignalMetadata).min_val === 0 &&
    (signal as AlertSignalMetadata).max_val === 1 &&
    !isEnumSignalMetadata(signal)
  );
};

const isNumericalSignalMetadata = (signal: SignalMetadata): signal is NumericalSignalMetadata => {
  return !isEnumSignalMetadata(signal) && !isAlertSignalMetadata(signal);
};

type SignalMetadata = EnumSignalMetadata | NumericalSignalMetadata | AlertSignalMetadata;

export type { SignalMetadata };

export { isAlertSignalMetadata, isEnumSignalMetadata, isNumericalSignalMetadata };
