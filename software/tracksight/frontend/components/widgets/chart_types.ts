import { SeriesData } from "@/lib/seriesData";
import { SignalType } from "@/lib/types/Signal";

type SeriesBase = {
	label: string;
	timestamps: number[];
};

export type NumericalSeries = SeriesBase & {
	type: SignalType.NUMERICAL;
	color: string;
	min: number;
	max: number;
	data: SeriesData;
};

export type EnumSeries = SeriesBase & {
	type: SignalType.ENUM;
	data: Array<number>;
	enumValuesToNames: Record<number, string[]>; // map of series index to unique enum values
};
export type Series = NumericalSeries | EnumSeries;
type ChartDataBase = {
	type: SignalType;
};
type ChartDataNumerical = ChartDataBase & {
	type: SignalType.NUMERICAL;
	all_series: Array<NumericalSeries>;
};
type ChartDataEnum = ChartDataBase & {
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
