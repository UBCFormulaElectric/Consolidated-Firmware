import socket from "@/lib/realtime/socket";
import { ENDPOINTS } from "@/lib/api/endpoints";

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

async function postSubscriptionAction<TBody extends object>(url: string, body: TBody, allowedStatuses: number[] = []) {
    console.log(url, body);
    const response = await fetch(url, {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(body),
    });

    if (!response.ok && !allowedStatuses.includes(response.status)) {
        throw new Error(`Request failed for ${url}: ${response.status}`);
    }

    return response.json();
}

const subscribeToSignal = async (signalName: string) => {
    const sid = await waitForSocketId();
    return postSubscriptionAction(ENDPOINTS.subscribe, {
        sid,
        signal: signalName,
    });
};

const unsubscribeFromSignal = async (signalName: string) => {
    const sid = await waitForSocketId();
    return postSubscriptionAction(ENDPOINTS.unsubscribe, {
        sid,
        signal: signalName,
    });
};

export { subscribeToSignal, unsubscribeFromSignal, waitForSocketId };
