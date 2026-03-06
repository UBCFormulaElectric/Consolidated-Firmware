import { EnumSeries, NumericalSeries } from "@/components/widgets/CanvasChartTypes";
import { EnumSignalConfig, NumericalSignalConfig, WidgetSignalConfig } from "@/components/widgets/WidgetTypes";
import { SignalType } from "../types/Signal";
import { SeriesData } from "../seriesData";

export type SignalStoreReturnType<T extends WidgetSignalConfig> = T extends EnumSignalConfig
  ? EnumSeries
  : T extends NumericalSignalConfig
  ? NumericalSeries
  : never;

type SignalStorageEntry<T extends WidgetSignalConfig> = {
  isSubscribed: boolean;
  error: unknown | null;
  data: SignalStoreReturnType<T>;
  // NOTE(evan): Type safety slop 
  storeType: T extends EnumSignalConfig
  ? SignalType.ENUM
  : T extends NumericalSignalConfig
  ? SignalType.NUMERICAL
  : never;
}

abstract class SignalStore {
  protected storage: {
    [signalName: string]: SignalStorageEntry<WidgetSignalConfig>;
  };
  protected subscriberCounts: { [signalName: string]: number };
  protected updateWithTimestamp: (timestamp: number) => void;

  constructor(updateWithTimestamp: (timestamp: number) => void) {
    this.storage = {};
    this.subscriberCounts = {};
    this.updateWithTimestamp = updateWithTimestamp;
  }

  abstract getReferenceToSignal<T extends WidgetSignalConfig>(signal: T): SignalStoreReturnType<T>;
  abstract purgeReferenceToSignal(signal: WidgetSignalConfig): void;

  private createSignalDataEntry<T extends WidgetSignalConfig>(signal: T): void {
    const storageBase = {
      error: null,
      isSubscribed: true,
      storeType: signal.type,
    };

    switch (signal.type) {
      case SignalType.ENUM:
        this.storage[signal.signal_name] = {
          ...storageBase,
          data: {
            enumValuesToNames: {},
            data: [],
            timestamps: [],
            label: signal.signal_name,
          } satisfies EnumSeries,
        };
        break;
      case SignalType.NUMERICAL:
        this.storage[signal.signal_name] = {
          ...storageBase,
          data: {
            data: new SeriesData(),
            timestamps: [],
            label: signal.signal_name,
          } satisfies NumericalSeries,
        }
        break;
    }

    this.subscriberCounts[signal.signal_name] = 0;
  }

  protected getOrCreateSignalData<T extends WidgetSignalConfig>(signal: T): SignalStorageEntry<T> {
    if (!this.storage[signal.signal_name]) this.createSignalDataEntry(signal);

    return this.getSignalData(signal)!;
  }


  getSignalData<T extends WidgetSignalConfig>(signal: T): SignalStorageEntry<T> | undefined {
    return this.storage[signal.signal_name];
  }

  incrementSubscribers(signalName: string): void {
    if (!this.subscriberCounts[signalName]) this.subscriberCounts[signalName] = 0;

    this.subscriberCounts[signalName] += 1;
  }

  decrementSubscribers(signalName: string): boolean {
    if (!this.subscriberCounts[signalName]) return false;

    this.subscriberCounts[signalName] -= 1;

    return this.subscriberCounts[signalName] <= 0;
  }

  getSubscriberCount(signalName: string): number {
    return this.subscriberCounts[signalName] || 0;
  }

  markAsUnsubscribed(signalName: string): void {
    if (!this.storage[signalName]) return;

    this.storage[signalName].isSubscribed = false;
  }

  removeSignal(signalName: string): void {
    delete this.storage[signalName];
    delete this.subscriberCounts[signalName];
  }

  setError(signalName: string, error: unknown): void {
    if (!this.storage[signalName]) return;

    this.storage[signalName].error = error;
  }

  addDataPoint(signalName: string, timestamp: number, value: number): void {
    this.updateWithTimestamp(timestamp);
    const entry = this.storage[signalName];

    if (!entry) return;

    switch (entry.storeType) {
      case SignalType.ENUM:
        entry.data.data.push(value);
        entry.data.timestamps.push(timestamp);
        break;
      case SignalType.NUMERICAL:
        entry.data.data.push(value);
        entry.data.timestamps.push(timestamp);
        break;
    }
  }
}

export default SignalStore;
