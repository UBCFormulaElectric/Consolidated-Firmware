"use client";

// TODO(evan): All these functions should be split into separate files.
//             I just didn't want to name a file for these stupid global
//             stores.

import { useEffect, useState } from "react";
import { fetchSignalMetadata } from "@/lib/api/signals";
import { API_BASE_URL } from "@/lib/constants";

/**
 * Map to hold subscribers for data updates, keyed by signal name. 
 */
const dataSubscribers: Map<string, Set<() => void>> = new Map();

/**
 * Map to hold number of subscribers for signal data updates, keyed by signal name. 
 * When the count reaches zero, we stop fetching data for that signal to save resources.
 */
const signalDataSubscribers = new Map<string, number>();

/**
 * Global store for signal data, keyed by signal name.
 */
const signalDataStore: Record<string, any[]> = {};

// NOTE(evan): Fill the signalDataStore with all of our known signals so that
//             javascript can hopefully optimize access to the object.
const signals = fetchSignalMetadata(API_BASE_URL);

signals
    .then(metadataList => {
        metadataList.forEach(metadata => {
            signalDataStore[metadata.name] = [];
        });
    })
    .catch(err => {
        console.error("Failed to fetch signal metadata:", err);
    });

const subscribeToSignalData = (signalName: string) => {
    const currentCount = signalDataSubscribers.get(signalName) || 0;
    signalDataSubscribers.set(signalName, currentCount + 1);

    if (currentCount === 0) {
        // TODO(evan): Start fetching data for this signal.
    }
};

const unsubscribeFromSignalData = (signalName: string) => {
    const currentCount = signalDataSubscribers.get(signalName) || 0;

    if (currentCount > 1) {
        signalDataSubscribers.set(signalName, currentCount - 1);

        return;
    }

    signalDataSubscribers.delete(signalName);
};

/**
 * Hook to get the data version for a list of signals, used to allow
 * for re-rendering without having to create new arrays which would
 * trigger unnecessary garbage collection.
 * 
 * @param signals - Array of signal names.
 */
const useDataVersion = (signals: string[]) => {
    const [dataVersion, setDataVersion] = useState(0);

    useEffect(() => {
        // NOTE(evan): When data for any of the subscribed signals updates, we increment
        //             the data version to trigger a re-render.
        const notify = () => setDataVersion(prev => prev + 1);

        signals.forEach(signalName => {
            if (!dataSubscribers.has(signalName)) {
                dataSubscribers.set(signalName, new Set());
            }

            dataSubscribers.get(signalName)!.add(notify);
        });

        return () => {
            signals.forEach(signalName => {
                dataSubscribers.get(signalName)?.delete(notify);
            });
        };
    }, [signals]);

    return dataVersion;
}

/**
 * Hook that subscribes to signal data updates and returns a stable reference to the signal's data array.
 * 
 * The returned array reference never changes - it's mutated in place when data updates occur.
 * To trigger re-renders on data updates, use the `useDataVersion` hook alongside this.
 * 
 * @param signalName - Name of the signal to subscribe to.
 * 
 * @returns A stable reference to the signal's data array.
 */
const useSignalDataRef = (signalName: string): readonly any[] => {
    useEffect(() => {
        subscribeToSignalData(signalName);

        return () => {
            unsubscribeFromSignalData(signalName);
        };
    }, [signalName]);

    return signalDataStore[signalName];
}

export {
    useSignalDataRef,
    useDataVersion,
}
