import React, { memo, useEffect, useRef } from "react";

import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { SignalDataStoreProvider } from "@/lib/contexts/signalStores/SignalStoreContext";
import MockSignalStore from "@/lib/signals/MockSignalStore";

const MockSignalStoreProvider = memo(({ children }: { children: React.ReactNode }) => {
    const { updateWithTimestamp } = useSyncedGraph();
    const mockSignalStore = useRef<MockSignalStore>(null!);

    if (!mockSignalStore.current) {
        mockSignalStore.current = new MockSignalStore(updateWithTimestamp);
    }

    useEffect(() => {
        const store = mockSignalStore.current;
        store.start();
        return () => store.stop();
    }, []);

    return <SignalDataStoreProvider signalStore={mockSignalStore}>{children}</SignalDataStoreProvider>;
});

export { MockSignalStoreProvider };
