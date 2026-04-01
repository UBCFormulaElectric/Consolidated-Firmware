"use client"

import { useEffect, useMemo, useState } from "react";

import CalendarDropdown from "@/components/icons/CalendarDropdown";
import SyncedGraphContainer from "@/components/SyncedGraphContainer";
import { DisplayControlProvider } from "@/components/PausePlayControl";

const getUtcDayRange = (date: Date) => {
    const year = date.getUTCFullYear();
    const month = date.getUTCMonth();
    const day = date.getUTCDate();

    const min = Date.UTC(year, month, day, 0, 0, 0, 0) / 1000;
    const max = Date.UTC(year, month, day + 1, 0, 0, 0, 0) / 1000;

    return { min, max };
};

export default function Historic() {
    const [selectedDate, setSelectedDate] = useState<Date>(new Date());
    const selectedRange = useMemo(() => getUtcDayRange(selectedDate), [selectedDate]);

    return (
        <DisplayControlProvider>
            <div>{selectedDate.getDate()}</div>
            <div>{selectedRange.min} {selectedRange.max}</div>
            <div className="pt-14 h-screen">
                <div className="ml-4 mb-4">
                    <CalendarDropdown selectedDate={selectedDate} onDateSelect={setSelectedDate} />
                </div>

                <div className="mx-4 mb-3 text-sm font-medium text-gray-600">
                    Time axis shown in UTC.
                </div>

                <div className="mx-4 h-[70vh] rounded-2xl border border-gray-200 bg-white">
                    <SyncedGraphContainer initialTimeRange={selectedRange}>
                        <div className="h-full w-full" />
                    </SyncedGraphContainer>
                </div>
            </div>
        </DisplayControlProvider>
    );
}
