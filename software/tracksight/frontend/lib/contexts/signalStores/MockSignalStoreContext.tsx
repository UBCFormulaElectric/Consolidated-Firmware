import React, { memo, useRef } from "react";

import MockSignalStore from "@/lib/signals/MockSignalStore";
import { SignalDataStoreProvider } from "@/lib/contexts/signalStores/SignalStoreContext";

const MockSignalStoreProvider = memo(({ children }: { children: React.ReactNode }) => {
    const mockSignalStore = useRef(new MockSignalStore());

    return (
        <SignalDataStoreProvider signalStore={mockSignalStore}>
            {children}
        </SignalDataStoreProvider>
    );
});

export { MockSignalStoreProvider };
