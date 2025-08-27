import { useEffect, useState } from "react";
import { io, Socket } from "socket.io-client";
import { BACKEND_URL, DEBUG, MAX_RECONNECT_ATTEMPTS } from "../SignalConfig";

export function useSocket() {
	// Socket stored in ref to prevent re-initialization on dependency changes
	const [socket, _setSocket] = useState<Socket>(io(BACKEND_URL, {
		transports: ["websocket", "polling"],
		autoConnect: true,
		reconnectionAttempts: MAX_RECONNECT_ATTEMPTS,
		timeout: 10000,
	}));

	// Initialize socket exactly once
	useEffect(() => {
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
		return () => {
			socket.disconnect();
		};
	}, [socket]); // Initialize only once and disconnect on unmount

	return socket;
}