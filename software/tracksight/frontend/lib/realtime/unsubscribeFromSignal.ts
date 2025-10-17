import { API_BASE_URL, IS_DEBUG } from "../constants";
import socket, { pendingUnsubscriptions } from "./socket";

const unsubscribeFromSignal = async (signalName: string) => {
  if (!socket.connected) {
    IS_DEBUG &&
      console.log(
        `%c[Unsubscribe] %cSocket not connected, caching unsubscription for: ${signalName}`,
        "color: #ebcb8b; font-weight: bold;",
        "color: #d08770;"
      );
    pendingUnsubscriptions.add(signalName);

    return { cached: true };
  }

  const response = await fetch(`${API_BASE_URL}/unsubscribe`, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify({
      sid: socket.id,
      signal: signalName,
    }),
  });

  return response.json();
};

export default unsubscribeFromSignal;
