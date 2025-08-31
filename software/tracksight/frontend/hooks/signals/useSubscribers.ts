import { useCallback, useEffect, useMemo, useRef } from "react";
import { Socket } from "socket.io-client";
import { BACKEND_URL, DEBUG } from "../SignalConfig";

export function useSubscribers(socket: Socket, pruneSignalData: (name: string) => void, clearAllData: () => void) {
	const signalSubscriberCount = useRef<Record<string, number>>({});
	// Pending debounce unsubscribe timers (name -> timeout id)
	const pendingUnsubs = useRef<Record<string, ReturnType<typeof setTimeout>>>({});
	const UNSUB_DEBOUNCE_MS = 200; // delay to absorb StrictMode double-invoke
	const getSignalRefCount = useCallback((name: string) => signalSubscriberCount.current[name] || 0, []);

	const activeSignals = useMemo(() => Object.keys(signalSubscriberCount.current).filter(k => signalSubscriberCount.current[k] > 0), [signalSubscriberCount])
	const isSubscribed = useCallback((signalName: string) => signalName in signalSubscriberCount.current, [signalSubscriberCount]);

	// SUB MANAGEMENT
	const subscribeToSignal = useCallback((signalName: string) => {
		const name = signalName.trim();

		// If there is a pending debounced unsubscribe, cancel it (StrictMode double-mount case)
		if (pendingUnsubs.current[name]) {
			clearTimeout(pendingUnsubs.current[name]);
			delete pendingUnsubs.current[name];
			DEBUG && console.log(`[subs] cancel pending unsubscribe for ${name}`);
		}
		const prev = signalSubscriberCount.current[name] || 0;
		const next = prev + 1;
		signalSubscriberCount.current[name] = next;
		if (DEBUG) console.log(`[subs] subscribe(${name}) count ${prev} -> ${next}`);

		// Only inform backend when transitioning 0 -> 1 AND no pending cancelled unsubscribe (prev===0)
		if (prev > 0) {
			DEBUG && console.log(`[subs] ${name} already has refs (${next}), skip backend subscribe`);
			return;
		}

		if (!socket.connected || !socket.id) {
			DEBUG && console.warn("[subs] Socket not connected; deferring backend subscribe for", name);
			return; // Will be re-requested on reconnect handler
		}

		const sid = socket.id;
		fetch(`${BACKEND_URL}/subscribe`, {
			method: "POST",
			headers: { "Content-Type": "application/json" },
			body: JSON.stringify({ sid, signal: name })
		}).then(async (res) => {
			if (!res.ok) {
				const txt = await res.text();
				// Swallow benign duplicate subscribe
				if (res.status === 400 && /already subscribed/i.test(txt)) {
					DEBUG && console.log(`[subs] duplicate subscribe ignored for ${name}`);
					return;
				}
				throw new Error(`Subscribe failed: ${res.status} ${txt}`);
			}
			DEBUG && console.log(`[subs] backend subscribed ${sid} -> ${name}`);
		}).catch(e => DEBUG && console.warn("[subs] Subscribe request error", e));
	}, [socket]);

	const unsubscribeFromSignal = useCallback((signalName: string) => {
		const name = signalName.trim();

		if (!(name in signalSubscriberCount.current)) {
			throw new Error(`Cannot unsubscribe from ${name} - not subscribed`);
		}
		if (signalSubscriberCount.current[name] <= 0) {
			throw new Error(`Cannot unsubscribe from ${name} - ref count is already 0`);
		}

		const prev = signalSubscriberCount.current[name];
		const next = prev - 1;
		signalSubscriberCount.current[name] = next;
		if (DEBUG) console.log(`[subs] unsubscribe(${name}) count ${prev} -> ${next}`);

		if (next === 0) {
			// Schedule debounced backend unsubscribe
			if (pendingUnsubs.current[name]) {
				clearTimeout(pendingUnsubs.current[name]);
			}
			pendingUnsubs.current[name] = setTimeout(() => {
				// If resubscribed, skip
				if ((signalSubscriberCount.current[name] || 0) > 0) {
					DEBUG && console.log(`[subs] abort debounce unsubscribe for ${name} (ref revived)`);
					delete pendingUnsubs.current[name];
					return;
				}
				delete pendingUnsubs.current[name];
				// Remove from map only now to keep potential quick revive logic clean
				delete signalSubscriberCount.current[name];
				if (socket.connected && socket.id) {
					const sid = socket.id;
					fetch(`${BACKEND_URL}/unsubscribe`, {
						method: "POST",
						headers: { "Content-Type": "application/json" },
						body: JSON.stringify({ sid, signal: name })
					}).then(async (res) => {
						if (!res.ok) {
							const txt = await res.text();
							// Swallow benign not-subscribed errors
							if (res.status === 400 && /(not subscribed)|(is not subscribed)/i.test(txt)) {
								DEBUG && console.log(`[subs] benign unsubscribe miss ignored for ${name}`);
								return;
							}
							throw new Error(`Unsubscribe failed: ${res.status} ${txt}`);
						}
						DEBUG && console.log(`[subs] backend unsubscribed ${sid} -> ${name}`);
					}).catch(e => DEBUG && console.warn("Unsubscribe request error", e));
				} else {
					DEBUG && console.warn("[subs] Socket not connected; cannot inform backend of unsubscription for", name);
				}
				// Clean up local data
				pruneSignalData(name);
			}, UNSUB_DEBOUNCE_MS);
			DEBUG && console.log(`[subs] scheduled debounce unsubscribe for ${name} in ${UNSUB_DEBOUNCE_MS}ms`);
		} else {
			DEBUG && console.log(`[subs] ${name} still has refs (${next}), skip backend unsubscribe`);
		}
	}, [pruneSignalData, socket]);

	const clearAllSubscriptions = useCallback(() => {
		DEBUG && console.log("Clearing all subscriptions and data");
		// Batch unsubscribe all active signals
		if (!socket.connected || !socket.id) {
			throw new Error("Socket is not connected, cannot clear subscriptions");
		}
		const sid = socket.id;
		// Batch over active signals
		activeSignals.forEach((sig) => {
			fetch(`${BACKEND_URL}/unsubscribe`, {
				method: "POST",
				headers: { "Content-Type": "application/json" },
				body: JSON.stringify({ sid, signal: sig })
			}).catch(() => { });
		});
		// Clear all state
		clearAllData();
	}, [socket, activeSignals, clearAllData]);

	// Restore subscriptions on reconnect (server clears subscriptions on disconnect)
	useEffect(() => {
		const onConnect = () => {
			if (!socket.id) return;
			const sid = socket.id;
			const names = Object.keys(signalSubscriberCount.current);
			DEBUG && console.log(`[subs] socket connected (${sid}); resubscribing ${names.length} signals`, names);
			names.forEach((name) => {
				fetch(`${BACKEND_URL}/subscribe`, {
					method: "POST",
					headers: { "Content-Type": "application/json" },
					body: JSON.stringify({ sid, signal: name })
				}).then(res => {
					if (!res.ok) DEBUG && console.warn(`[subs] resubscribe failed for ${name}: ${res.status}`);
				}).catch((e) => { DEBUG && console.warn(`[subs] resubscribe error for ${name}`, e); });
			});
		};
		socket.on("connect", onConnect);
		return () => { socket.off("connect", onConnect); };
	}, [socket]);

	return { activeSignals, subscribeToSignal, unsubscribeFromSignal, getSignalRefCount, isSubscribed, clearAllSubscriptions }
}