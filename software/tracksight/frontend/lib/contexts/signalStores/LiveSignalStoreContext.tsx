import React, { memo, useRef } from "react";

import LiveSignalStore from "@/lib/signals/LiveSignalStore";
import { SignalDataStoreProvider } from "@/lib/contexts/signalStores/SignalStoreContext";
import useSubscribeToSignal from "@/lib/mutations/useSubscribeToSignal";
import useUnsubscribeToSignal from "@/lib/mutations/useUnsubscribeToSignal";

const LiveSignalStoreProvider = memo(({ children }: { children: React.ReactNode }) => {
    const subscribeToSignalMutation = useSubscribeToSignal();
    const unsubscribeFromSignalMutation = useUnsubscribeToSignal();

    const liveSignalStore = useRef(new LiveSignalStore(
        subscribeToSignalMutation.mutate,
        unsubscribeFromSignalMutation.mutate
    ));

    return (
        <SignalDataStoreProvider signalStore={liveSignalStore}>
            {children}
        </SignalDataStoreProvider>
    );
});

export { LiveSignalStoreProvider };
