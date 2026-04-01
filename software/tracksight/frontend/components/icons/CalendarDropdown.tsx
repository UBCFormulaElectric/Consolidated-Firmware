"use client";

import { CalendarIcon, ChevronDown, ChevronLeft, ChevronRight } from "lucide-react";
import { useMemo, useState } from "react";

import { Popover, PopoverContent, PopoverTrigger } from "@/components/ui/popover";
import { cn } from "@/lib/utils";

const MONTH_NAMES = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December",];

const WEEKDAY_LABELS = ["S", "M", "T", "W", "T", "F", "S"];

const getOrdinalDay = (day: number) => {
    const mod100 = day % 100;

    if (mod100 >= 11 && mod100 <= 13) {
        return `${day}th`;
    }

    switch (day % 10) {
        case 1:
            return `${day}st`;
        case 2:
            return `${day}nd`;
        case 3:
            return `${day}rd`;
        default:
            return `${day}th`;
    }
};

const getCalendarDays = (displayMonth: Date) => {
    const year = displayMonth.getFullYear();
    const month = displayMonth.getMonth();

    const firstDay = new Date(year, month, 1);
    const firstVisibleDay = new Date(firstDay);
    firstVisibleDay.setDate(firstDay.getDate() - firstDay.getDay());

    return Array.from({ length: 42 }, (_, index) => {
        const date = new Date(firstVisibleDay);
        date.setDate(firstVisibleDay.getDate() + index);

        return {
            date,
            inCurrentMonth: date.getMonth() === month,
        };
    });
};

type CalendarDropdownProps = {
    selectedDate?: Date;
    onDateSelect?: (date: Date) => void;
};

const CalendarDropdown = ({ selectedDate, onDateSelect }: CalendarDropdownProps) => {
    const [calendarOpen, setCalendarOpen] = useState<boolean>(false);
    const [internalSelectedDate, setInternalSelectedDate] = useState<Date>(new Date());
    const [displayMonth, setDisplayMonth] = useState<Date>(new Date());

    const activeSelectedDate = selectedDate ?? internalSelectedDate;

    const calendarDays = useMemo(() => getCalendarDays(displayMonth), [displayMonth]);


    const handleDateSelect = (date: Date) => {
        if (!selectedDate) {
            setInternalSelectedDate(date);
        }
        onDateSelect?.(date);
        setCalendarOpen(false);
    };

    const handleMonthChange = (month: number) => {
        setDisplayMonth(new Date(displayMonth.getFullYear(), month, 1));
    };

    const handleYearChange = (year: number) => {
        setDisplayMonth(new Date(year, displayMonth.getMonth(), 1));
    };

    return (
        <Popover open={calendarOpen} onOpenChange={setCalendarOpen}>
            <PopoverTrigger asChild>
                <button
                    type="button"
                    className="flex min-w-84 items-center justify-between gap-4 rounded-3xl border border-gray-200 bg-white px-5 py-4 text-left shadow-[0_14px_40px_rgba(15,23,42,0.12)] transition-colors hover:bg-gray-50 cursor-grab"
                >
                    <div className="flex items-center gap-4">
                        <div className="flex size-11 items-center justify-center rounded-2xl bg-gray-100 text-gray-900">
                            <CalendarIcon className="size-6" strokeWidth={2.2} />
                        </div>
                        <div>
                            <div className="text-[1.9rem] font-semibold leading-none text-gray-950">
                                {MONTH_NAMES[activeSelectedDate.getMonth()]} {getOrdinalDay(activeSelectedDate.getDate())}, {activeSelectedDate.getFullYear()}
                            </div>
                        </div>
                    </div>
                    <ChevronDown
                        className={cn(
                            "size-6 shrink-0 text-gray-500 transition-transform",
                            calendarOpen && "rotate-180"
                        )}
                        strokeWidth={2.25}
                    />
                </button>
            </PopoverTrigger>

            <PopoverContent
                align="start"
                sideOffset={14}
                className="w-150 rounded-[2rem] border border-gray-200 bg-white p-6 shadow-[0_28px_70px_rgba(15,23,42,0.16)]"
            >
                <div>
                    <section className="min-w-0 rounded-[1.6rem] bg-gray-100 p-4">
                        <div className="mb-4 flex items-center gap-3">
                            <button
                                type="button"
                                className="flex size-10 items-center justify-center rounded-full bg-white text-gray-700 transition-colors hover:bg-gray-200"
                                onClick={() =>
                                    setDisplayMonth(
                                        new Date(displayMonth.getFullYear(), displayMonth.getMonth() - 1, 1)
                                    )
                                }
                            >
                                <ChevronLeft className="size-5" />
                            </button>

                            <select
                                className="h-11 min-w-0 flex-1 rounded-xl border border-gray-200 bg-white px-4 text-base font-medium text-gray-900 outline-none"
                                value={displayMonth.getMonth()}
                                onChange={(event) => handleMonthChange(Number(event.target.value))}
                            >
                                {MONTH_NAMES.map((month, index) => (
                                    <option key={month} value={index}>
                                        {month}
                                    </option>
                                ))}
                            </select>

                            <select
                                className="h-11 w-28 rounded-xl border border-gray-200 bg-white px-4 text-base font-medium text-gray-900 outline-none"
                                value={displayMonth.getFullYear()}
                                onChange={(event) => handleYearChange(Number(event.target.value))}
                            >
                                {Array.from({ length: 7 }, (_, index) => 2023 + index).map((year) => (
                                    <option key={year} value={year}>
                                        {year}
                                    </option>
                                ))}
                            </select>

                            <button
                                type="button"
                                className="flex size-10 items-center justify-center rounded-full bg-white text-gray-700 transition-colors hover:bg-gray-200"
                                onClick={() =>
                                    setDisplayMonth(
                                        new Date(displayMonth.getFullYear(), displayMonth.getMonth() + 1, 1)
                                    )
                                }
                            >
                                <ChevronRight className="size-5" />
                            </button>
                        </div>

                        <div className="grid grid-cols-7 gap-y-3 rounded-[1.35rem] bg-white px-3 py-4">
                            {WEEKDAY_LABELS.map((label, index) => (
                                <div
                                    key={`${label}-${index}`}
                                    className="pb-1 text-center text-sm font-semibold uppercase tracking-[0.18em] text-gray-500"
                                >
                                    {label}
                                </div>
                            ))}

                            {calendarDays.map(({ date, inCurrentMonth }) => {
                                const isSelected = date.getFullYear() === activeSelectedDate.getFullYear() && date.getMonth() === activeSelectedDate.getMonth() && date.getDate() === activeSelectedDate.getDate();

                                return (
                                    <button
                                        key={date.toISOString()}
                                        type="button"
                                        className={cn(
                                            "mx-auto flex size-11 items-center justify-center rounded-full text-lg font-semibold transition-colors",
                                            isSelected && "bg-blue-600 text-white shadow-[0_8px_18px_rgba(37,99,235,0.35)]",
                                            !isSelected && inCurrentMonth && "text-gray-800 hover:bg-gray-100",
                                            !isSelected && !inCurrentMonth && "text-gray-300 hover:bg-gray-50"
                                        )}
                                        onClick={() => handleDateSelect(date)}
                                    >
                                        {date.getDate()}
                                    </button>
                                );
                            })}
                        </div>
                    </section>
                </div>
            </PopoverContent>
        </Popover>
    );
};

export default CalendarDropdown;
