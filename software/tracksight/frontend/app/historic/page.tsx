"use client"

import { useMemo, useState } from "react";

import DataDashboard from "@/components/DataDashboard";
import CalendarDropdown from "@/components/icons/CalendarDropdown";
import { ViewportLockButton, DisplayControlProvider } from "@/components/PausePlayControl";
import SyncedGraphContainer from "@/components/SyncedGraphContainer";
import { WidgetManager, useWidgetManager } from "@/components/widgets/WidgetManagerContext";
import { WidgetAdder } from "@/app/live/WidgetAdder";
import { HistoricalSignalStoreProvider } from "@/lib/contexts/signalStores/HistoricalSignalStoreContext";

const HISTORIC_WIDGET_STORAGE_KEY = "tracksight_historic_widgets_config_v1";
const RESOLUTION_OPTIONS = ["1ms", "2s", "5s", "10s", "30s", "1m", "5m"] as const; // change later, we'll probably aim for x points on screen or something
type Resolution = typeof RESOLUTION_OPTIONS[number];

// TOOO: KEEP TRACK OF LOADED TIME RANGE AND ALLOW USER TO LOAD MORE DATA BUT DIFF THE TIMESTAMPS

const getUtcDayRange = (date: Date) => {
    const year = date.getUTCFullYear();
    const month = date.getUTCMonth();
    const day = date.getUTCDate();

    const min = Date.UTC(year, month, day, 0, 0, 0, 0);
    const max = Date.UTC(year, month, day + 1, 0, 0, 0, 0);

    return { min, max };
};

function HistoricContent(props: { selectedRange: { min: number; max: number }; resolution: Resolution }) {
    const { selectedRange, resolution } = props;
    const { widgets } = useWidgetManager();

    return (
        <HistoricalSignalStoreProvider
            startUtcMs={selectedRange.min}
            endUtcMs={selectedRange.max}
            resolution={resolution}
        >
            {widgets.length === 0 ? (
                <div className="grid h-full place-items-center text-gray-500">
                    Select signals by adding a widget and choosing signals.
                </div>
            ) : (
                <DataDashboard />
            )}
            <WidgetAdder />
        </HistoricalSignalStoreProvider>
    );
}

export default function Historic() {
    const [selectedDate, setSelectedDate] = useState<Date>(new Date());
    const [resolution, setResolution] = useState<Resolution>("10s");
    const selectedRange = useMemo(() => getUtcDayRange(selectedDate), [selectedDate]);

    return (
        <DisplayControlProvider>
            <div className="pt-14 h-screen bg">
                <div className="mx-4 mb-4 flex items-center gap-4">
                    <CalendarDropdown selectedDate={selectedDate} onDateSelect={setSelectedDate} />
                    <label className="text-sm font-medium text-gray-700">
                        Aggregate
                        <select
                            className="ml-2 rounded-lg border border-gray-300 bg-white px-3 py-2 text-sm text-gray-900"
                            value={resolution}
                            onChange={(event) => setResolution(event.target.value as Resolution)}
                        >
                            {RESOLUTION_OPTIONS.map((option) => (
                                <option key={option} value={option}>{option}</option>
                            ))}
                        </select>
                    </label>
                    <ViewportLockButton />
                </div>

                <div className="mx-4 mb-3 text-sm font-medium text-gray-600">
                    Time axis shown in UTC!! ts aint pst.
                </div>

                <SyncedGraphContainer initialTimeRange={selectedRange}>
                    <WidgetManager storageKey={HISTORIC_WIDGET_STORAGE_KEY}>
                        <HistoricContent selectedRange={selectedRange} resolution={resolution} />
                    </WidgetManager>
                </SyncedGraphContainer>
            </div>
        </DisplayControlProvider>
    );
}
