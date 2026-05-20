import { AlertSeries, LODAwareEnumSeries, LODAwareNumericalSeries, NumericalSeries } from "@/components/widgets/CanvasChartTypes";
import { SignalMetadata, SignalType } from "../types/Signal";
import { SeriesData } from "../seriesData";

export type SignalStoreReturnType<T extends SignalMetadata> = T["type"] extends SignalType.ENUM
  ? LODAwareEnumSeries
  : T["type"] extends SignalType.NUMERICAL
  ? LODAwareNumericalSeries
  : T["type"] extends SignalType.ALERT
  ? NumericalSeries
  : never

type SignalStorageEntry = {
  isSubscribed: boolean;
  error: unknown | null;
} & (
    {
      data: NumericalSeries
      storeType: SignalType.ALERT
    }
    | {
      data: LODAwareNumericalSeries
      storeType: SignalType.NUMERICAL
    }
    | {
      data: LODAwareEnumSeries
      storeType: SignalType.ENUM
    }
  )

abstract class SignalStore {
  protected storage: {
    [signalName: string]: SignalStorageEntry
  };
  protected alertDataStorage: {
    [signalName: string]: AlertSeries;
  }

  protected subscriberCounts: { [signalName: string]: number };
  protected updateWithTimestamp: (timestamp: number) => void;

  constructor(updateWithTimestamp: (timestamp: number) => void) {
    this.storage = {};
    this.subscriberCounts = {};
    this.alertDataStorage = {};

    this.updateWithTimestamp = updateWithTimestamp;
  }

  abstract getReferenceToSignal<T extends SignalMetadata>(signal: T): SignalStoreReturnType<T>;
  abstract purgeReferenceToSignal(signal: SignalMetadata): void;

  protected addAlertDataPoint(signalName: string, timestamp: number, value: number): void {
    this.updateWithTimestamp(timestamp);
    const entry = this.alertDataStorage[signalName];

    if (!entry) {
      this.alertDataStorage[signalName] = {
        label: signalName,
        data: [value],
        timestamps: [timestamp],
      };
    } else {
      entry.data.push(value);
      entry.timestamps.push(timestamp);
    }
  }

  public getAlertData(): { [signalName: string]: AlertSeries } {
    return this.alertDataStorage;
  }

  private createSignalDataEntry<T extends SignalMetadata>(signal: T): void {
    const storageBase = {
      error: null,
      isSubscribed: true,
    };

    switch (signal.type) {
      case SignalType.ENUM:
        this.storage[signal.name] = {
          ...storageBase,
          data: {
            enumValuesToNames: {},
            label: signal.name,
            lods: [{
              sampleIntervalMs: 0, data: [], timestamps: []
            }]
          } satisfies LODAwareEnumSeries,
          storeType: SignalType.ENUM
        } satisfies SignalStorageEntry
        break;
      case SignalType.NUMERICAL:
        this.storage[signal.name] = {
          ...storageBase,
          data: {
            label: signal.name,
            lods: [{ sampleIntervalMs: 0, data: new SeriesData(), timestamps: [] }],
          } satisfies LODAwareNumericalSeries,
          storeType: SignalType.NUMERICAL,
        }
        break;
    }

    this.subscriberCounts[signal.name] = 0;
  }

  protected getOrCreateSignalData<T extends SignalMetadata>(signal: T): Extract<SignalStorageEntry, { storeType: T["type"] }> {
    if (!this.storage[signal.name]) this.createSignalDataEntry(signal);

    return this.getSignalData(signal)!;
  }

  getSignalData<T extends SignalMetadata>(signal: T): Extract<SignalStorageEntry, { storeType: T["type"] }> | undefined {
    const signalDataToReturn = this.storage[signal.name];

    if (!signalDataToReturn) return undefined;

    if (signalDataToReturn.storeType !== signal.type) return undefined;

    // NOTE(evan): We can assume this `any` cast is safe because we already force `storeType` and `type`
    //             to be equal so we know it's the correct return type Typescript just can't do runtime
    //             discriminated union scoping like that.
    return signalDataToReturn as any;
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

  addDataPointAtLOD(signalName: string, lod: number, sampleIntervalMs: number, timestamp: number, value: number): void {
    const entry = this.storage[signalName];

    if (!entry || entry.storeType === SignalType.ALERT) return;

    while (entry.data.lods.length <= lod) {
      if (entry.storeType === SignalType.ENUM) {
        entry.data.lods.push({ sampleIntervalMs: 0, data: [], timestamps: [] });
      } else {
        entry.data.lods.push({ sampleIntervalMs: 0, data: new SeriesData(), timestamps: [] });
      }
    }

    entry.data.lods[lod].sampleIntervalMs = sampleIntervalMs;
    entry.data.lods[lod].data.push(value);
    entry.data.lods[lod].timestamps.push(timestamp);
  }

  addDataPoint(signalName: string, timestamp: number, value: number): void {
    this.updateWithTimestamp(timestamp);
    const entry = this.storage[signalName];

    if (!entry) return;

    switch (entry.storeType) {
      case SignalType.ENUM:
        entry.data.lods[0].data.push(value);
        entry.data.lods[0].timestamps.push(timestamp);
        break;
      case SignalType.NUMERICAL:
        entry.data.lods[0].data.push(value);
        entry.data.lods[0].timestamps.push(timestamp);
        break;
    }
  }
}

export default SignalStore;
