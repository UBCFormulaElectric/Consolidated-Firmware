import io from "socket.io-client";

import { API_BASE_URL, MAX_RECCONECTION_ATTEMPTS } from "@/lib/constants";

const socket = io(API_BASE_URL, {
  transports: ["websocket", "polling"],
  autoConnect: true,
  reconnectionAttempts: MAX_RECCONECTION_ATTEMPTS,
  timeout: 10_000,
});

export default socket;
