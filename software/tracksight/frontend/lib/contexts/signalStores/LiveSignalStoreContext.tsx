import React, { memo, useEffect, useRef } from "react";

import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { SignalDataStoreProvider } from "@/lib/contexts/signalStores/SignalStoreContext";
import useSubscribeToSignal from "@/lib/mutations/useSubscribeToSignal";
import useUnsubscribeToSignal from "@/lib/mutations/useUnsubscribeToSignal";
import LiveSignalStore from "@/lib/signals/LiveSignalStore";

type SignalMutationCallbacks = {
    onSuccess?: () => void;
    onError?: (error: unknown) => void;
};

function withPerCallCallbacks(mutateAsync: (signalName: string) => Promise<unknown>) {
    return (signalName: string, callbacks?: SignalMutationCallbacks) => {
        mutateAsync(signalName).then(
            () => callbacks?.onSuccess?.(),
            (error: unknown) => callbacks?.onError?.(error)
        );
    };
}

const LiveSignalStoreProvider = memo(({ children }: { children: React.ReactNode }) => {
    const subscribeToSignalMutation = useSubscribeToSignal();
    const unsubscribeFromSignalMutation = useUnsubscribeToSignal();
    const { updateWithTimestamp } = useSyncedGraph();
    const liveSignalStore = useRef<LiveSignalStore>(null!);

    if (!liveSignalStore.current) {
        liveSignalStore.current = new LiveSignalStore(updateWithTimestamp, withPerCallCallbacks(subscribeToSignalMutation.mutateAsync), withPerCallCallbacks(unsubscribeFromSignalMutation.mutateAsync));
    }

    useEffect(() => {
        const store = liveSignalStore.current;
        store.attach();
        return () => store.detach();
    }, []);

    return <SignalDataStoreProvider signalStore={liveSignalStore}>{children}</SignalDataStoreProvider>;
});

export { LiveSignalStoreProvider };
