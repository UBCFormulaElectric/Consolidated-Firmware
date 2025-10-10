import { useCallback, useEffect, useRef } from "react";
import { DataPoint, DEBUG, DEFAULT_MAX_DATA_POINTS, SignalDataStore } from "../SignalConfig";

// Optional onFlush callback notifies when a batch has been flushed so consumers can trigger re-render.
export function useSignalData(onFlush?: () => void) {
	// Optimized data storage
	const dataStore = useRef<SignalDataStore>(new SignalDataStore());
	// OPTIMIZED BATCHING SYSTEM using data store
	const pendingDataUpdates = useRef<DataPoint[]>([]);
	const batchUpdateTimer = useRef<NodeJS.Timeout | null>(null);
	const rafUpdateTimer = useRef<number | null>(null);
	const BATCH_INTERVAL_MS = 50; // Fallback for setTimeout
	const MAX_BATCH_SIZE = 100; // Max data points per batch
	const currentTimeUpdateCounter = useRef(0);
	const CURRENT_TIME_UPDATE_FREQUENCY = 10; // Update currentTime every 10th batch
	const lastBatchTime = useRef(Date.now());

	// Cleanup on unmount
	useEffect(() => {
		return () => {
			// Clear batch timers
			if (batchUpdateTimer.current) {
				clearTimeout(batchUpdateTimer.current);
			}
			if (rafUpdateTimer.current) {
				cancelAnimationFrame(rafUpdateTimer.current);
			}
		};
	}, []);

	// OPTIMIZED BATCHED DATA UPDATE FUNCTION with RAF and smart batching
	const flushPendingDataUpdates = useCallback(() => {
		if (pendingDataUpdates.current.length === 0) return;

		const updates = [...pendingDataUpdates.current];
		pendingDataUpdates.current = [];

		// Track batching performance for adaptive behavior
		const now = Date.now();
		const timeSinceLastBatch = now - lastBatchTime.current;
		lastBatchTime.current = now;

		// Group updates by signal for more efficient processing
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

		// Add all updates to the optimized data store
		signalGroups.forEach((points) => {
			points.forEach((point) => {
				dataStore.current.addDataPoint(point);
			});
		});

		// Adaptive currentTime updates based on data flow
		currentTimeUpdateCounter.current++;
		const isHighFrequency = timeSinceLastBatch < 100; // High frequency if batches < 100ms apart
		const updateFrequency = isHighFrequency ? CURRENT_TIME_UPDATE_FREQUENCY * 2 : CURRENT_TIME_UPDATE_FREQUENCY;

		if (currentTimeUpdateCounter.current >= updateFrequency) {
			currentTimeUpdateCounter.current = 0;
		}

		// Clear timers
		batchUpdateTimer.current = null;
		rafUpdateTimer.current = null;
		// Notify consumer (SignalContext) so it can bump a version state to re-render subscribers
		if (onFlush) onFlush();
	}, []);

	// ENHANCED BATCHED DATA ADDITION with RAF and size-based flushing
	const addDataPoint = useCallback((dataPoint: DataPoint) => {
		pendingDataUpdates.current.push(dataPoint);

		// Immediate flush if batch size exceeds threshold (prevents memory buildup)
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

		// Prefer requestAnimationFrame for smooth 60fps updates
		if (!rafUpdateTimer.current && !batchUpdateTimer.current) {
			if (typeof requestAnimationFrame !== 'undefined') {
				rafUpdateTimer.current = requestAnimationFrame(flushPendingDataUpdates);
			} else {
				// Fallback to setTimeout for environments without RAF
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
	}, [dataStore.current]);

	return { dataStore, addDataPoint, pruneSignalData, pruneData, clearAllData };
}