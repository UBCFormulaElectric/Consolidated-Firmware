/**
 * Defines the concrete interface for data that is consumed by widget renderers
 */

import { SeriesData } from "@/lib/seriesData";
import { SignalType } from "@/lib/types/Signal";

type SeriesBase = {
    label: string;
    timestamps: number[];
};

export type NumericalSeries = SeriesBase & {
    data: SeriesData;
};

export type LODAwareSeries<T = {}> = {
    label: string;
    lods: (LODBase & T)[];
};

type LODBase = {
    sampleIntervalMs: number;
    timestamps: number[];
};

export type LODAwareNumericalSeries = LODAwareSeries<{
    data: SeriesData;
}> & {
    label: string;
};

export type AlertSeries = SeriesBase & {
    data: Array<number>;
};

export type LODAwareEnumSeries = LODAwareSeries<{
    data: Array<number>;
}> & {
    enumValuesToNames: Record<number, number[]>;
};

export type EnumSeries = SeriesBase & {
    data: Array<number>;
    enumValuesToNames: Record<number, string[]>; // map of series index to unique enum values
};

export type Series = NumericalSeries | EnumSeries;

type ChartDataBase = {
    type: SignalType;
};

export type ChartDataNumerical = ChartDataBase & {
    type: SignalType.NUMERICAL;
    all_series: Array<NumericalSeries>;
};

export type ChartDataEnum = ChartDataBase & {
    type: SignalType.ENUM;
    all_series: Array<EnumSeries>;
};

export type ChartData = ChartDataNumerical | ChartDataEnum;

export interface ChartLayout {
    minTime: number;
    timeRange: number;
    chartWidth: number;
    paddingLeft: number;
}
