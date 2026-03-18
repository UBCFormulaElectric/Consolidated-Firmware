import { useCallback, useEffect, useRef } from "react";

import { DataPoint, DEBUG, DEFAULT_MAX_DATA_POINTS, SignalDataStore } from "@/lib/SignalConfig";

export function useSignalData(onFlush?: () => void) {
  const dataStore = useRef<SignalDataStore>(new SignalDataStore());
  const pendingDataUpdates = useRef<DataPoint[]>([]);
  const batchUpdateTimer = useRef<NodeJS.Timeout | null>(null);
  const rafUpdateTimer = useRef<number | null>(null);
  const BATCH_INTERVAL_MS = 50;
  const MAX_BATCH_SIZE = 100;
  const currentTimeUpdateCounter = useRef(0);
  const CURRENT_TIME_UPDATE_FREQUENCY = 10;
  const lastBatchTime = useRef(Date.now());

  useEffect(() => {
    return () => {
      if (batchUpdateTimer.current) {
        clearTimeout(batchUpdateTimer.current);
      }
      if (rafUpdateTimer.current) {
        cancelAnimationFrame(rafUpdateTimer.current);
      }
    };
  }, []);

  const flushPendingDataUpdates = useCallback(() => {
    if (pendingDataUpdates.current.length === 0) return;

    const updates = [...pendingDataUpdates.current];
    pendingDataUpdates.current = [];

    const now = Date.now();
    const timeSinceLastBatch = now - lastBatchTime.current;
    lastBatchTime.current = now;

    const signalGroups = new Map<string, DataPoint[]>();
    updates.forEach((point) => {
      const signalName = point.name;
      if (signalName && !signalGroups.has(signalName)) {
        signalGroups.set(signalName, []);
      }
      if (signalName) {
        signalGroups.get(signalName)!.push(point);
      }
    });

    signalGroups.forEach((points) => {
      points.forEach((point) => {
        dataStore.current.addDataPoint(point);
      });
    });

    currentTimeUpdateCounter.current++;
    const isHighFrequency = timeSinceLastBatch < 100;
    const updateFrequency = isHighFrequency
      ? CURRENT_TIME_UPDATE_FREQUENCY * 2
      : CURRENT_TIME_UPDATE_FREQUENCY;

    if (currentTimeUpdateCounter.current >= updateFrequency) {
      currentTimeUpdateCounter.current = 0;
    }

    batchUpdateTimer.current = null;
    rafUpdateTimer.current = null;
    if (onFlush) onFlush();
  }, [onFlush]);

  const addDataPoint = useCallback((dataPoint: DataPoint) => {
    pendingDataUpdates.current.push(dataPoint);

    if (pendingDataUpdates.current.length >= MAX_BATCH_SIZE) {
      if (rafUpdateTimer.current) {
        cancelAnimationFrame(rafUpdateTimer.current);
        rafUpdateTimer.current = null;
      }
      if (batchUpdateTimer.current) {
        clearTimeout(batchUpdateTimer.current);
        batchUpdateTimer.current = null;
      }
      flushPendingDataUpdates();
      return;
    }

    if (!rafUpdateTimer.current && !batchUpdateTimer.current) {
      if (typeof requestAnimationFrame !== "undefined") {
        rafUpdateTimer.current = requestAnimationFrame(flushPendingDataUpdates);
      } else {
        batchUpdateTimer.current = setTimeout(flushPendingDataUpdates, BATCH_INTERVAL_MS);
      }
    }
  }, [flushPendingDataUpdates]);

  const pruneSignalData = useCallback((name: string) => {
    if (DEBUG) console.log(`Pruning all data for signal: ${name}`);
    dataStore.current.removeSignalData(name);
  }, []);

  const pruneData = useCallback((max = DEFAULT_MAX_DATA_POINTS) => {
    if (DEBUG) console.log(`Pruning data to last ${max} points`);
    dataStore.current.pruneToLastN(max);
  }, []);

  const clearAllData = useCallback(() => {
    if (DEBUG) console.log("Clearing data");
    dataStore.current.clear();
  }, []);

  return { dataStore, addDataPoint, pruneSignalData, pruneData, clearAllData };
}
