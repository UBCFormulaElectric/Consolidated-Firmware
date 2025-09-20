import { useEffect, useMemo } from "react";
import { io, Socket } from "socket.io-client";
import { BACKEND_URL, DEBUG, MAX_RECONNECT_ATTEMPTS } from "../SignalConfig";

// Module-level singleton to survive re-mounts/HMR and avoid duplicate connections
let sharedSocket: Socket | null = null;
let listenersAttached = false;

function getSharedSocket(): Socket {
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