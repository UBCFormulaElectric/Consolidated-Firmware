"use client";

import { createContext, ReactNode, useContext, useEffect, useState } from "react";

export type Timezone = "UTC" | "America/Vancouver" | "America/Detroit";

export const TIMEZONE_OPTIONS: { value: Timezone; label: string }[] = [
    { value: "UTC", label: "UTC" },
    { value: "America/Vancouver", label: "PDT (Vancouver)" },
    { value: "America/Detroit", label: "EDT (Michigan) " },
];

type TimezoneContextType = {
    timezone: Timezone;
    setTimezone: (tz: Timezone) => void;
};

const TimezoneContext = createContext<TimezoneContextType | undefined>(undefined);

export function TimezoneProvider({ children }: { children: ReactNode }) {
    const [timezone, setTimezoneState] = useState<Timezone>("UTC");

    useEffect(() => {
        const stored = localStorage.getItem("tracksight-timezone") as Timezone | null;
        if (stored && TIMEZONE_OPTIONS.some((o) => o.value === stored)) {
            setTimezoneState(stored);
        }
    }, []);

    const setTimezone = (tz: Timezone) => {
        setTimezoneState(tz);
        localStorage.setItem("tracksight-timezone", tz);
    };

    return <TimezoneContext.Provider value={{ timezone, setTimezone }}>{children}</TimezoneContext.Provider>;
}

export function useTimezone() {
    const context = useContext(TimezoneContext);
    if (!context) {
        throw new Error("useTimezone must be used within a TimezoneProvider");
    }
    return context;
}
