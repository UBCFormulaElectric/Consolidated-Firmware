// Signal type classification
export enum SignalType {
  Numerical = "numerical",
  Enumeration = "enumeration",
}

// Signal metadata interface
export interface SignalMeta {
  name: string;
  unit: string;
  cycle_time_ms?: number;
  msg_id: number;
  msg_name: string;
  enum?: {
    name: string,
    items: Record<string, string>
  };
}

// Data point interface
export interface DataPoint {
  time: number | string;
  name?: string;
  value?: number | string;
  [signalName: string]: number | string | undefined;
}

// Optimized time-series data structures
export interface TimeSeriesPoint {
  timestamp: number;
  value: number | string;
  signalName: string;
}

export class TimeRangeIndex {
  private timestamps: number[] = [];
  private indices: number[] = [];

  insert(timestamp: number, index: number): void {
    // Binary search for insertion point
    let left = 0;
    let right = this.timestamps.length;

    while (left < right) {
      const mid = Math.floor((left + right) / 2);
      if (this.timestamps[mid] < timestamp) {
        left = mid + 1;
      } else {
        right = mid;
      }
    }

    this.timestamps.splice(left, 0, timestamp);
    this.indices.splice(left, 0, index);
  }

  findRange(startTime: number, endTime: number): number[] {
    const startIndex = this.findFirstGreaterOrEqual(startTime);
    const endIndex = this.findLastLessOrEqual(endTime);

    if (startIndex === -1 || endIndex === -1 || startIndex > endIndex) {
      return [];
    }

    return this.indices.slice(startIndex, endIndex + 1);
  }

  private findFirstGreaterOrEqual(timestamp: number): number {
    let left = 0;
    let right = this.timestamps.length;

    while (left < right) {
      const mid = Math.floor((left + right) / 2);
      if (this.timestamps[mid] < timestamp) {
        left = mid + 1;
      } else {
        right = mid;
      }
    }

    return left < this.timestamps.length ? left : -1;
  }

  private findLastLessOrEqual(timestamp: number): number {
    let left = 0;
    let right = this.timestamps.length - 1;
    let result = -1;

    while (left <= right) {
      const mid = Math.floor((left + right) / 2);
      if (this.timestamps[mid] <= timestamp) {
        result = mid;
        left = mid + 1;
      } else {
        right = mid - 1;
      }
    }

    return result;
  }

  clear(): void {
    this.timestamps = [];
    this.indices = [];
  }
}

export class SignalDataStore {
  private dataPoints: TimeSeriesPoint[] = [];
  private timeIndex: TimeRangeIndex;
  private signalIndices: Map<string, number[]> = new Map();

  constructor() {
    this.timeIndex = new TimeRangeIndex();
  }

  addDataPoint(point: DataPoint): void {
    const timestamp = typeof point.time === 'string' ? parseFloat(point.time) : point.time;
    const signalName = point.name || 'unknown';
    const value = point.value !== undefined ? point.value : 0;

    const tsPoint: TimeSeriesPoint = {
      timestamp,
      signalName,
      value
    };

    // Add to data array
    const index = this.dataPoints.length;
    this.dataPoints.push(tsPoint);

    // Update time index
    this.timeIndex.insert(timestamp, index);

    // Update signal index
    if (!this.signalIndices.has(signalName)) {
      this.signalIndices.set(signalName, []);
    }
    this.signalIndices.get(signalName)!.push(index);
  }

  getDataInTimeRange(startTime: number, endTime: number): DataPoint[] {
    const indices = this.timeIndex.findRange(startTime, endTime);

    return indices
      .filter(index => index < this.dataPoints.length)
      .map(index => {
        const point = this.dataPoints[index];
        return {
          time: point.timestamp,
          name: point.signalName,
          value: point.value
        } as DataPoint;
      });
  }

  getSignalData(signalName: string): DataPoint[] {
    const indices = this.signalIndices.get(signalName) || [];

    return indices
      .filter(index => index < this.dataPoints.length)
      .map(index => {
        const point = this.dataPoints[index];
        return {
          time: point.timestamp,
          name: point.signalName,
          value: point.value
        } as DataPoint;
      });
  }

  getAllData(): DataPoint[] {
    return this.dataPoints.map(point => ({
      time: point.timestamp,
      name: point.signalName,
      value: point.value
    } as DataPoint));
  }

  getNumericalData(): DataPoint[] {
    return this.dataPoints
      .filter(point => typeof point.value === 'number')
      .map(point => ({
        time: point.timestamp,
        name: point.signalName,
        value: point.value
      } as DataPoint));
  }

  getEnumData(): DataPoint[] {
    return this.dataPoints
      .filter(point => typeof point.value === 'string')
      .map(point => ({
        time: point.timestamp,
        name: point.signalName,
        value: point.value
      } as DataPoint));
  }

  removeSignalData(signalName: string): void {
    // Get indices for this signal
    const indicesToRemove = this.signalIndices.get(signalName) || [];

    if (indicesToRemove.length === 0) return;

    // Remove from data array (in reverse order to maintain indices)
    indicesToRemove.sort((a, b) => b - a).forEach(index => {
      this.dataPoints.splice(index, 1);
    });

    // Rebuild indices since array positions changed
    this.rebuildIndices();

    // Remove signal from indices
    this.signalIndices.delete(signalName);
  }

  clear(): void {
    this.dataPoints = [];
    this.timeIndex.clear();
    this.signalIndices.clear();
  }

  getDataCount(): number {
    return this.dataPoints.length;
  }

  // Prune data to keep only the last N points
  pruneToLastN(maxPoints: number): void {
    if (this.dataPoints.length <= maxPoints) return;

    const pointsToRemove = this.dataPoints.length - maxPoints;
    this.dataPoints.splice(0, pointsToRemove);

    // Rebuild all indices since positions changed
    this.rebuildIndices();
  }

  private rebuildIndices(): void {
    // Clear existing indices
    this.timeIndex.clear();
    this.signalIndices.clear();

    // Rebuild from current data
    this.dataPoints.forEach((point, index) => {
      this.timeIndex.insert(point.timestamp, index);

      if (!this.signalIndices.has(point.signalName)) {
        this.signalIndices.set(point.signalName, []);
      }
      this.signalIndices.get(point.signalName)!.push(index);
    });
  }
}

// Default maximum number of data points to keep
export const DEFAULT_MAX_DATA_POINTS = 1000;
export const BACKEND_URL =
  typeof window !== "undefined"
    ? `http://${window.location.hostname}:5000`
    : `http://localhost:5000`;
export const MAX_RECONNECT_ATTEMPTS = 5;
export const RECONNECT_INTERVAL = 3000; // 3 seconds
export const DEBUG = true; // Set to false to disable debug logs
