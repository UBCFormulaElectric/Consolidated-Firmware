import { SeriesData } from "@/lib/seriesData";
import { SignalType } from "@/lib/types/Signal";

type SeriesBase = {
	label: string;
	timestamps: number[];
};

export type NumericalSeries = SeriesBase & {
	data: SeriesData;
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
// export type ChartDataOf<T extends WidgetData> = Extract<ChartData, { type: T["type"] }>;

export interface ChartLayout {
	minTime: number;
	timeRange: number;
	chartWidth: number;
	paddingLeft: number;
}
