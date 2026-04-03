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

// TOOO: KEEP TRACK OF LOADED TIME RANGE AND ALLOW USER TO LOAD MORE DATA BUT DIFF THE TIMESTAMPS

const getUtcDayRange = (date: Date) => {
    const year = date.getUTCFullYear();
    const month = date.getUTCMonth();
    const day = date.getUTCDate();

    const min = Date.UTC(year, month, day, 0, 0, 0, 0);
    const max = Date.UTC(year, month, day + 1, 0, 0, 0, 0);

    return { min, max };
};

function HistoricContent(props: { selectedRange: { min: number; max: number }; }) {
    const { selectedRange } = props;
    const { widgets } = useWidgetManager();

    return (
        <HistoricalSignalStoreProvider
            startUtcMs={selectedRange.min}
            endUtcMs={selectedRange.max}
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
    const selectedRange = useMemo(() => getUtcDayRange(selectedDate), [selectedDate]);

    return (
        <DisplayControlProvider>
            <div className="pt-14 h-screen bg">
                <div className="mx-4 mb-4 flex items-center gap-4">
                    <CalendarDropdown selectedDate={selectedDate} onDateSelect={setSelectedDate} />

                    <ViewportLockButton />
                </div>

                <div className="mx-4 mb-3 text-sm font-medium text-gray-600">
                    Time axis shown in UTC!! ts aint pst.
                </div>

                <SyncedGraphContainer initialTimeRange={selectedRange}>
                    <WidgetManager storageKey={HISTORIC_WIDGET_STORAGE_KEY}>
                        <HistoricContent selectedRange={selectedRange} />
                    </WidgetManager>
                </SyncedGraphContainer>
            </div>
        </DisplayControlProvider>
    );
}
