import { API_BASE_URL } from "@/lib/constants";
import socket from "@/lib/realtime/socket";

const SOCKET_CONNECT_TIMEOUT_MS = 10_000;

async function waitForSocketId(): Promise<string> {
  if (socket.connected && socket.id) {
    return socket.id;
  }

  await new Promise<void>((resolve, reject) => {
    const timeoutId = window.setTimeout(() => {
      cleanup();
      reject(new Error("Timed out waiting for socket connection"));
    }, SOCKET_CONNECT_TIMEOUT_MS);

    const handleConnect = () => {
      cleanup();
      resolve();
    };

    const handleConnectError = (error: unknown) => {
      cleanup();
      reject(error instanceof Error ? error : new Error("Socket connection failed"));
    };

    const cleanup = () => {
      window.clearTimeout(timeoutId);
      socket.off("connect", handleConnect);
      socket.off("connect_error", handleConnectError);
    };

    socket.on("connect", handleConnect);
    socket.on("connect_error", handleConnectError);
  });

  if (!socket.id) {
    throw new Error("Socket connected without an SID");
  }

  return socket.id;
}

async function postSubscriptionAction<TBody extends object>(
  path: string,
  body: TBody,
  allowedStatuses: number[] = []
) {

  console.log(API_BASE_URL, path, body);
  const response = await fetch(`${API_BASE_URL}${path}`, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify(body),
  });

  if (!response.ok && !allowedStatuses.includes(response.status)) {
    throw new Error(`Request failed for ${path}: ${response.status}`);
  }

  return response.json();
}

const subscribeToSignal = async (signalName: string) => {
  const sid = await waitForSocketId();
  return postSubscriptionAction("/subscribe", {
    sid,
    signal: signalName,
  });
};

const unsubscribeFromSignal = async (signalName: string) => {
  const sid = await waitForSocketId();
  return postSubscriptionAction("/unsubscribe", {
    sid,
    signal: signalName,
  });
};

const pauseSignalStream = async () => {
  const sid = await waitForSocketId();
  return postSubscriptionAction(`/${sid}/pause`, {}, [209]);
};

const resumeSignalStream = async () => {
  const sid = await waitForSocketId();
  return postSubscriptionAction(`/${sid}/play`, {}, [209]);
};

export { pauseSignalStream, resumeSignalStream, subscribeToSignal, unsubscribeFromSignal, waitForSocketId };
