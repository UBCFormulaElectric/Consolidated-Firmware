import SignalStore from "@/lib/signals/SignalStore";
import { SignalMetadata, SignalType } from "../types/Signal";
import propagateHaar, { HaarLodBuffer } from "../utils/propagateHaar";
import propagateMode, { ModeLodBuffer } from "../utils/propagateMode";

export const MOCK_STATES = [ // needed to hardcode for widgetadder
  "IDLE", "ACTIVE", "ERROR", "WAITING", "CHARGING", "SKIBIDI",
];

export const ALERT_SIGNALS = [
  "CPU Usage",
  "Memory Usage",
  "Disk Space",
  "Network Traffic",
  "Temperature",
  "Power Consumption",
  "Fan Speed",
  "GPU Usage",
  "Battery Level",
  "Process Count",
  "Thread Count",
  "IO Wait",
  "Page Faults",
  "Context Switches",
  "Interrupts",
  "System Load",
  "Swap Usage",
  "Latency",
  "Error Rate",
  "Throughput",
]

const INITIAL_DATA_POINTS = 0;

const NUM_LOD_LEVELS = 15;

function generateRandomNumericalValue(time: number, index: number = 0, min: number, max: number) {
  if (min !== undefined && max !== undefined) {
    const range = max - min;
    // Generate a wave within the range
    const normalized = (Math.sin(time / 1000 + index) + 1) / 2; // 0 to 1

    // Add some noise
    const noise = (Math.random() - 0.5) * 0.1; // -0.05 to 0.05
    let n = normalized + noise;
    n = Math.max(0, Math.min(1, n));
    return min + n * range;
  }

  return (
    Math.sin(time / 1000 + index) * 50 + Math.random() * 10 + 50 + index * 20
  );
}

function generateRandomEnumValue() {
  const v = Math.floor(Math.random() * MOCK_STATES.length);
  return {
    name: MOCK_STATES[v],
    idx: v,
  };
}

function generateRandomAlertValue(prev: number) {
  const shouldChange = Math.random() < 0.01;
  if (!shouldChange) return prev;

  const change = prev === 0 ? 1 : 0;
  return change;
}

class MockSignalStore extends SignalStore {
  private signalSubscriptionInterval: Map<string, number>;
  private lodBuffers: Map<string, (HaarLodBuffer | ModeLodBuffer)[]>;

  constructor(
    updateWithTimestamp: (timestamp: number) => void,
  ) {
    super(updateWithTimestamp);

    this.signalSubscriptionInterval = new Map();
    this.lodBuffers = new Map();

    ALERT_SIGNALS.forEach(signalName => {
      let previousValue = 0;

      const intervalId = setInterval(() => {
        const now = Date.now();
        const value = generateRandomAlertValue(previousValue);
        previousValue = value;

        this.addAlertDataPoint(signalName, now, value);
      }, 1);

      for (let i = 0; i < INITIAL_DATA_POINTS; i++) {
        const timestamp = Date.now() - (INITIAL_DATA_POINTS - i);
        const value = generateRandomAlertValue(previousValue);
        previousValue = value;
        this.addAlertDataPoint(signalName, timestamp, value);
      }

      this.signalSubscriptionInterval.set(signalName, intervalId as unknown as number);
    });
  }

  // FIXME(evan): Type stuff I can't be bothered to do right now
  getReferenceToSignal<T extends SignalMetadata>(signal: T) {
    const signalData = this.getOrCreateSignalData(signal);
    this.incrementSubscribers(signal.name);

    if (this.getSubscriberCount(signal.name) !== 1) return signalData.data as any;

    // NOTE(evan): When a browser tab isn't focused setInterval is throttled to run once per second.
    //             This can make the renderers appear frozen if they rely on frequent updates. In
    //             production when using an actual signal source this shouldn't be an issue.
    if (signal.type === SignalType.NUMERICAL) {
      const { min_val, max_val } = signal;
      const intervalId = setInterval(() => {
        const now = Date.now();
        const value = generateRandomNumericalValue(now, 0, min_val, max_val);
        this.addDataPoint(signal.name, now, value);

        propagateHaar(
          this.lodBuffers.get(signal.name) as HaarLodBuffer[] || [],
          0,
          now,
          value,
          (level, intervalMs, timestamp, value) => {
            this.addDataPointAtLOD(signal.name, level, intervalMs, timestamp, value);
          },
          NUM_LOD_LEVELS
        );
      }, 1) as unknown as number;

      this.signalSubscriptionInterval.set(signal.name, intervalId);
      this.lodBuffers.set(signal.name, new Array(NUM_LOD_LEVELS).fill(null));

      for (let i = 0; i < INITIAL_DATA_POINTS; i++) {
        const timestamp = Date.now() - (INITIAL_DATA_POINTS - i);
        const value = generateRandomNumericalValue(timestamp, 0, min_val, max_val);
        this.addDataPoint(signal.name, timestamp, value);

        propagateHaar(
          this.lodBuffers.get(signal.name) as HaarLodBuffer[] || [],
          0,
          timestamp,
          value,
          (level, intervalMs, timestamp, value) => {
            this.addDataPointAtLOD(signal.name, level - 1, intervalMs, timestamp, value);
          },
          NUM_LOD_LEVELS
        );
      }
    } else if (signal.type === SignalType.ENUM) {
      const intervalId = setInterval(() => {
        const now = Date.now();
        const value = generateRandomEnumValue();
        this.addDataPoint(signal.name, now, value.idx);

        propagateMode(
          this.lodBuffers.get(signal.name) as ModeLodBuffer[] || [],
          0,
          now,
          { [value.idx]: 1 },
          (level, intervalMs, timestamp, value) => {
            this.addDataPointAtLOD(signal.name, level, intervalMs, timestamp, value);
          },
          NUM_LOD_LEVELS
        );
      }, 1) as unknown as number;
      this.signalSubscriptionInterval.set(signal.name, intervalId);
      this.lodBuffers.set(signal.name, new Array(NUM_LOD_LEVELS).fill([]));

      for (let i = 0; i < INITIAL_DATA_POINTS; i++) {
        const timestamp = Date.now() - (INITIAL_DATA_POINTS - i);
        const value = generateRandomEnumValue();
        this.addDataPointAtLOD(signal.name, 0, 1, timestamp, value.idx);

        propagateMode(
          this.lodBuffers.get(signal.name) as ModeLodBuffer[] || [],
          0,
          timestamp,
          { [value.idx]: 1 },
          (level, intervalMs, timestamp, value) => {
            this.addDataPointAtLOD(signal.name, level - 1, intervalMs, timestamp, value);
          },
          NUM_LOD_LEVELS
        );
      }
    }

    return signalData.data as any;
  }

  purgeReferenceToSignal<T extends SignalMetadata>(signal: T) {
    const shouldCleanup = this.decrementSubscribers(signal.name);

    if (!shouldCleanup) return;

    this.markAsUnsubscribed(signal.name);

    const intervalId = this.signalSubscriptionInterval.get(signal.name);
    if (intervalId) {
      clearInterval(intervalId);
      this.signalSubscriptionInterval.delete(signal.name);
    }
  }
}

export default MockSignalStore;
