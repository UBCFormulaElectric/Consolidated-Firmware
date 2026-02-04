import io from "socket.io-client";

import subscribeToSignal from "@/lib/api/subscribeToSignal";
import unsubscribeFromSignal from "@/lib/api/unsubscribeFromSignal";
import { API_BASE_URL, IS_DEBUG, MAX_RECCONECTION_ATTEMPTS } from "@/lib/constants";

const socket = io(API_BASE_URL, {
  transports: ["websocket", "polling"],
  autoConnect: true,
  reconnectionAttempts: MAX_RECCONECTION_ATTEMPTS,
  timeout: 10_000,
});

export const pendingSubscriptions = new Set<string>();
export const pendingUnsubscriptions = new Set<string>();

const processPendingOperations = async () => {
  if (pendingSubscriptions.size > 0) {
    IS_DEBUG &&
      console.log(
        `%c[Socket] %cProcessing ${pendingSubscriptions.size} pending subscriptions`,
        "color: #a3be8c; font-weight: bold;",
        "color: #88c0d0;"
      );

    Array.from(pendingSubscriptions).forEach(subscribeToSignal);
    pendingSubscriptions.clear();
  }

  if (pendingUnsubscriptions.size > 0) {
    IS_DEBUG &&
      console.log(
        `%c[Socket] %cProcessing ${pendingUnsubscriptions.size} pending unsubscriptions`,
        "color: #a3be8c; font-weight: bold;",
        "color: #88c0d0;"
      );

    Array.from(pendingUnsubscriptions).forEach(unsubscribeFromSignal);

    pendingUnsubscriptions.clear();
  }
};

socket.on("connect", () => {
  IS_DEBUG &&
    console.log(
      `%c[Socket Connected] %cID: ${socket.id}`,
      "color: #a3be8c; font-weight: bold;",
      "color: #88c0d0;"
    );

  processPendingOperations();
});

export default socket;
