import { useCallback, useMemo, useRef } from "react";
import { Socket } from "socket.io-client";
import { DEBUG } from "../SignalConfig";

export function useSubscribers(socket: Socket, pruneSignalData: (name: string) => void, clearAllData: () => void) {
	const signalSubscriberCount = useRef<Record<string, number>>({});
	const getSignalRefCount = useCallback((name: string) => signalSubscriberCount.current[name] || 0, []);

	const activeSignals = useMemo(() => Object.keys(signalSubscriberCount.current), [signalSubscriberCount])
	const isSubscribed = useCallback((signalName: string) => signalName in signalSubscriberCount.current, [signalSubscriberCount]);

	// SUB MANAGEMENT
	const subscribeToSignal = useCallback((signalName: string) => {
		const name = signalName.trim();

		// Skip if unmounted or already pending unsubscription
		// if (!isMounted.current || pendingUnsubscriptions.current.has(name)) {
		//   if (DEBUG)
		//     console.log(
		//       `Skipping subscribe for ${name} - unmounted or pending unsubscription`
		//     );
		//   return;
		// }
		signalSubscriberCount.current[name] = (signalSubscriberCount.current[name] || 0) + 1;
		socket.emit("sub", name);
	}, []);

	const unsubscribeFromSignal = useCallback((signalName: string) => {
		const name = signalName.trim();

		if (!(name in signalSubscriberCount.current)) {
			throw new Error(`Cannot unsubscribe from ${name} - not subscribed`);
		}
		if (signalSubscriberCount.current[name] <= 0) {
			throw new Error(`Cannot unsubscribe from ${name} - ref count is already 0`);
		}

		signalSubscriberCount.current[name] = signalSubscriberCount.current[name] - 1;

		if (signalSubscriberCount.current[name] == 0) {
			delete signalSubscriberCount.current[name];
			// Always emit unsubscription if connected, regardless of pause state
			// This ensures clean disconnection from the signal
			if (socket.active) socket.emit("unsub", name);
			// Only clean up data if explicitly requested (for true unsubscription, not pause)
			pruneSignalData(name);
		}
	}, [pruneSignalData]);

	const clearAllSubscriptions = useCallback(() => {
		DEBUG && console.log("Clearing all subscriptions and data");
		// Batch unsubscribe all active signals
		if (!socket.active) {
			throw new Error("Socket is not active, cannot clear subscriptions");
		}
		activeSignals.forEach((sig) => socket.emit("unsub", sig));
		// Clear all state
		clearAllData();
	}, [socket, activeSignals, clearAllData]);

	return { activeSignals, subscribeToSignal, unsubscribeFromSignal, getSignalRefCount, isSubscribed, clearAllSubscriptions }
}