import React, { memo, useRef } from "react";

import LiveSignalStore from "@/lib/signals/LiveSignalStore";
import { SignalDataStoreProvider } from "@/lib/contexts/signalStores/SignalStoreContext";
import useSubscribeToSignal from "@/lib/mutations/useSubscribeToSignal";
import useUnsubscribeToSignal from "@/lib/mutations/useUnsubscribeToSignal";
import { useSyncedGraph } from "@/components/SyncedGraphContainer";

const LiveSignalStoreProvider = memo(({ children }: { children: React.ReactNode }) => {
    const subscribeToSignalMutation = useSubscribeToSignal();
    const unsubscribeFromSignalMutation = useUnsubscribeToSignal();
    const { updateWithTimestamp } = useSyncedGraph();
    const liveSignalStore = useRef<LiveSignalStore>(null!);

    if (!liveSignalStore.current) {
        liveSignalStore.current = new LiveSignalStore(
            updateWithTimestamp,
            subscribeToSignalMutation.mutate,
            unsubscribeFromSignalMutation.mutate
        );
    }

    return (
        <SignalDataStoreProvider signalStore={liveSignalStore}>
            {children}
        </SignalDataStoreProvider>
    );
});

export { LiveSignalStoreProvider };
