import { WidgetSignalConfig } from "@/components/widgets/WidgetTypes";
import SignalStore from "@/lib/signals/SignalStore";

export const MOCK_STATES = [ // needed to hardcode for widgetadder
  "IDLE", "ACTIVE", "ERROR", "WAITING", "CHARGING", "SKIBIDI",
  "TOILET", "MORE", "SIGNALS", "TO", "TEST", "RANDOM",
  "ENUM", "COLOR", "GEN"
];

export const ALERT_SIGNALS = [
  "CPU Usage",
  "Memory Usage",
  "Disk Space",
  "Network Traffic",
  "Temperature",
  "Power Consumption",
  "Fan Speed",
]

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

class MockSignalStore extends SignalStore {
  private signalSubscriptionIntervals: Map<string, number>;

  constructor(
    updateWithTimestamp: (timestamp: number) => void,
  ) {
    super(updateWithTimestamp);

    this.signalSubscriptionIntervals = new Map();
  }

  getReferenceToSignal<T extends WidgetSignalConfig>(signal: T) {
    const signalData = this.getOrCreateSignalData(signal);
    this.incrementSubscribers(signal.signal_name);

    if (this.getSubscriberCount(signal.signal_name) !== 1) return signalData.data;

    let previousValue = 0;

    // NOTE(evan): When a browser tab isn't focused setInterval is throttled to run once per second.
    //             This can make the renderers appear frozen if they rely on frequent updates. In 
    //             production when using an actual signal source this shouldn't be an issue.
    const intervalId = setInterval(() => {
      const now = Date.now();

      if (signal.type === "numerical") {
        const { min, max } = signal;
        const value = generateRandomNumericalValue(now, 0, min, max);
        previousValue = value;
        this.addDataPoint(signal.signal_name, now, value);
      } else if (signal.type === "enum") {
        const value = generateRandomEnumValue();
        previousValue = value.idx;
        this.addDataPoint(signal.signal_name, now, value.idx);
      }
    }, 1);

    this.signalSubscriptionIntervals.set(signal.signal_name, intervalId as unknown as number);

    return signalData.data;
  }

  purgeReferenceToSignal<T extends WidgetSignalConfig>(signal: T) {
    const shouldCleanup = this.decrementSubscribers(signal.signal_name);

    if (!shouldCleanup) return;

    this.markAsUnsubscribed(signal.signal_name);

    clearInterval(this.signalSubscriptionIntervals.get(signal.signal_name));
    this.signalSubscriptionIntervals.delete(signal.signal_name);
  }
}

export default MockSignalStore;
