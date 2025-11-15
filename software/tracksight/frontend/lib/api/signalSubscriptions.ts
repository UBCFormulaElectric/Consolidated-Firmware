import { API_BASE_URL } from "@/lib/constants";
import socket from "@/lib/realtime/socket";

const subscribeToSignal = async (signalName: string) => {
  const response = await fetch(`${API_BASE_URL}/subscribe`, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify({
      sid: socket.id,
      signal: signalName,
    }),
  });

  if (!response.ok) {
    throw new Error(`Failed to subscribe to signal: ${signalName}`);
  }

  return response.json();
};

const unsubscribeFromSignal = async (signalName: string) => {
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

  if (!response.ok) {
    throw new Error(`Failed to unsubscribe from signal: ${signalName}`);
  }

  return response.json();
};

export { subscribeToSignal, unsubscribeFromSignal };
