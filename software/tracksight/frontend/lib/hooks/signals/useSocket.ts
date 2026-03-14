import { useEffect, useMemo } from "react";
import { io, Socket } from "socket.io-client";
import { BACKEND_URL, DEBUG, MAX_RECONNECT_ATTEMPTS } from "@/lib/SignalConfig";

// Module-level singleton to survive re-mounts/HMR and avoid duplicate connections
let sharedSocket: Socket | null = null;
let listenersAttached = false;

function getSharedSocket(): Socket {
	// Avoid creating a real socket on the server during SSR to prevent hydration mismatches
	if (typeof window === 'undefined') {
		// Return a minimal stub that matches the Socket interface we use
		// so that render-time reads like `socket.connected` are stable (false)
		const noop = () => { /* no-op */ };
		return {
			connected: false,
			id: undefined,
			on: noop as any,
			off: noop as any,
			emit: noop as any,
			connect: noop as any,
			disconnect: noop as any,
			// spread to satisfy index signature expectations
		} as unknown as Socket;
	}

	if (!sharedSocket) {
		sharedSocket = io(BACKEND_URL, {
			transports: ["websocket", "polling"],
			autoConnect: true,
			reconnectionAttempts: MAX_RECONNECT_ATTEMPTS,
			timeout: 10000,
		});
	}
	return sharedSocket;
}

export function useSocket() {
	// Grab the singleton once for this component instance
	const socket = useMemo(() => getSharedSocket(), []);

	// Attach listeners once to avoid stacking logs in StrictMode/HMR
	useEffect(() => {
		if (!listenersAttached) {
			socket.on("connect", () => {
				DEBUG && console.log("Socket connected");
			});
			socket.on("connect_error", (e) => {
				DEBUG && console.log("Socket error:", e);
			});
			socket.on("disconnect", (r) => {
				DEBUG && console.log("Socket disconnected:", r);
			});
			socket.on("sub_ack", () => DEBUG && console.log("Subscription acknowledged"));
			listenersAttached = true;
		}

		// Don't disconnect the singleton on unmount; keep it alive across re-mounts
		return () => { /* no-op */ };
	}, [socket]);

	return socket;
}