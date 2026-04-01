import React, { memo, useRef } from "react";

import MockSignalStore from "@/lib/signals/MockSignalStore";
import { SignalDataStoreProvider } from "@/lib/contexts/signalStores/SignalStoreContext";
import { useSyncedGraph } from "@/components/SyncedGraphContainer";

const MockSignalStoreProvider = memo(({ children }: { children: React.ReactNode }) => {
    const { updateWithTimestamp } = useSyncedGraph();
    const mockSignalStore = useRef<MockSignalStore>(null!);

    if (!mockSignalStore.current) {
        mockSignalStore.current = new MockSignalStore(updateWithTimestamp);
    }

    return (
        <SignalDataStoreProvider signalStore={mockSignalStore}>
            {children}
        </SignalDataStoreProvider>
    );
});

export { MockSignalStoreProvider };
