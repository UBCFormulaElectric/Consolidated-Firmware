type SignalMetadata = {
  name: string;
  min_val: number;
  max_val: number;
  unit?: string;
  enum?: {
    items: Record<number, string>;
    name: string;
  };
  tx_node: string;
  cycle_time_ms: number;
  id: number;
  msg_name: string;
};

export type { SignalMetadata };
