import { LODAwareAlertSeries, LODAwareEnumSeries, LODAwareNumericalSeries } from "@/components/widgets/CanvasChartTypes";
import { SignalMetadata, SignalType } from "../types/Signal";
import { SeriesData } from "../seriesData";
import { LevelPoint, markTilesCovered, mergeSortedPoints } from "./lodLevels";

export type SignalStoreReturnType<T extends SignalMetadata> = T["type"] extends SignalType.ENUM
  ? LODAwareEnumSeries
  : T["type"] extends SignalType.NUMERICAL
  ? LODAwareNumericalSeries
  : T["type"] extends SignalType.ALERT
  ? LODAwareAlertSeries
  : never

type SignalStorageEntry = {
  isSubscribed: boolean;
  error: unknown | null;
} & (
    {
      data: LODAwareAlertSeries
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
    | {
      data: LODAwareEnumSeries
      storeType: SignalType.BOOLEAN
    }
  )

type AnyLod = {
  sampleIntervalMs: number;
  timestamps: number[];
  coveredTiles?: Set<number>;
  data: SeriesData | number[];
};

abstract class SignalStore {
  protected storage: {
    [signalName: string]: SignalStorageEntry
  };

  // Names of the dynamically-discovered alert (fault) signals, so consumers like the alert
  // timeline can enumerate them without scanning all of `storage`.
  protected alertSignalNames: Set<string>;

  protected subscriberCounts: { [signalName: string]: number };
  protected updateWithTimestamp: (timestamp: number) => void;

  constructor(updateWithTimestamp: (timestamp: number) => void) {
    this.storage = {};
    this.subscriberCounts = {};
    this.alertSignalNames = new Set();

    this.updateWithTimestamp = updateWithTimestamp;
  }

  abstract getReferenceToSignal<T extends SignalMetadata>(signal: T): SignalStoreReturnType<T>;
  abstract purgeReferenceToSignal(signal: SignalMetadata): void;

  /** The alert (fault) series currently in the store, keyed by signal name. */
  public getAlertSeries(): { [signalName: string]: LODAwareAlertSeries } {
    const result: { [signalName: string]: LODAwareAlertSeries } = {};
    this.alertSignalNames.forEach((name) => {
      const entry = this.storage[name];
      if (entry && entry.storeType === SignalType.ALERT) {
        result[name] = entry.data;
      }
    });
    return result;
  }

  protected createSignalDataEntry<T extends SignalMetadata>(signal: T): void {
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
      case SignalType.BOOLEAN:
        this.storage[signal.name] = {
          ...storageBase,
          data: {
            enumValuesToNames: { },
            label: signal.name,
            lods: [{ sampleIntervalMs: 0, data: [], timestamps: [] }],
          } satisfies LODAwareEnumSeries,
          storeType: SignalType.BOOLEAN,
        };
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
      case SignalType.ALERT:
        this.storage[signal.name] = {
          ...storageBase,
          data: {
            label: signal.name,
            lods: [{ sampleIntervalMs: 0, data: [], timestamps: [] }],
          } satisfies LODAwareAlertSeries,
          storeType: SignalType.ALERT,
        };
        this.alertSignalNames.add(signal.name);
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
    this.alertSignalNames.delete(signalName);
  }

  setError(signalName: string, error: unknown): void {
    if (!this.storage[signalName]) return;

    this.storage[signalName].error = error;
  }

  private getOrCreateLevel(entry: SignalStorageEntry, resolutionMs: number): AnyLod {
    const lods = entry.data.lods as AnyLod[];

    const existing = lods.find((lod) => lod.sampleIntervalMs === resolutionMs);
    if (existing) {
      if (!existing.coveredTiles) existing.coveredTiles = new Set();
      return existing;
    }

    const placeholder = lods.length === 1 && lods[0].sampleIntervalMs === 0 && lods[0].timestamps.length === 0 ? lods[0] : undefined;
    if (placeholder) {
      placeholder.sampleIntervalMs = resolutionMs;
      placeholder.coveredTiles = new Set();
      return placeholder;
    }

    const newLod: AnyLod = {
      sampleIntervalMs: resolutionMs,
      timestamps: [],
      coveredTiles: new Set(),
      data: entry.storeType === SignalType.NUMERICAL ? new SeriesData() : [],
    };

    let insertAt = lods.length;
    for (let i = 0; i < lods.length; i++) {
      if (lods[i].sampleIntervalMs > resolutionMs) {
        insertAt = i;
        break;
      }
    }
    lods.splice(insertAt, 0, newLod);
    return newLod;
  }

  /**
   * Merge a fetched historical window into the LOD level for its resolution: mark the requested
   * span's tiles as covered (even where the backend returned no points), dedupe-merge the
   * points, and expand the global time range. Never clears — re-fetching an overlapping range
   * is idempotent.
   */
  mergeHistoricalPoints(signalName: string, resolutionMs: number, requestStartMs: number, requestEndMs: number, points: LevelPoint[]): void {
    const entry = this.storage[signalName];
    if (!entry || resolutionMs <= 0) return;

    const lod = this.getOrCreateLevel(entry, resolutionMs);
    markTilesCovered(lod.coveredTiles!, resolutionMs, requestStartMs, requestEndMs);

    const existingValues = lod.data instanceof SeriesData
      ? Array.from({ length: lod.data.length }, (_, i) => (lod.data as SeriesData).get(i))
      : (lod.data as number[]);

    const merged = mergeSortedPoints(lod.timestamps, existingValues, points);
    lod.timestamps = merged.timestamps;

    if (lod.data instanceof SeriesData) {
      const rebuilt = new SeriesData();
      merged.values.forEach((value) => rebuilt.push(value));
      lod.data = rebuilt;
    } else {
      const arr = lod.data as number[];
      arr.length = 0;
      merged.values.forEach((value) => arr.push(value));
    }

    points.forEach((point) => this.updateWithTimestamp(point.timestampMs));
  }

  addDataPointAtLOD(signalName: string, lod: number, sampleIntervalMs: number, timestamp: number, value: number): void {
    const entry = this.storage[signalName];

    if (!entry || entry.storeType === SignalType.ALERT) return;

    while (entry.data.lods.length <= lod) {
      if (entry.storeType === SignalType.ENUM || entry.storeType === SignalType.BOOLEAN) {
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
      case SignalType.BOOLEAN:
      case SignalType.ALERT:
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
