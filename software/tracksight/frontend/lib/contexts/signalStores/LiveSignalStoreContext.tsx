import React, { memo, useRef } from "react";

import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { SignalDataStoreProvider } from "@/lib/contexts/signalStores/SignalStoreContext";
import useSubscribeToSignal from "@/lib/mutations/useSubscribeToSignal";
import useUnsubscribeToSignal from "@/lib/mutations/useUnsubscribeToSignal";
import LiveSignalStore from "@/lib/signals/LiveSignalStore";

const LiveSignalStoreProvider = memo(({ children }: { children: React.ReactNode }) => {
    const subscribeToSignalMutation = useSubscribeToSignal();
    const unsubscribeFromSignalMutation = useUnsubscribeToSignal();
    const { updateWithTimestamp } = useSyncedGraph();
    const liveSignalStore = useRef<LiveSignalStore>(null!);

    if (!liveSignalStore.current) {
        liveSignalStore.current = new LiveSignalStore(updateWithTimestamp, subscribeToSignalMutation.mutate, unsubscribeFromSignalMutation.mutate);
    }

    return <SignalDataStoreProvider signalStore={liveSignalStore}>{children}</SignalDataStoreProvider>;
});

export { LiveSignalStoreProvider };
