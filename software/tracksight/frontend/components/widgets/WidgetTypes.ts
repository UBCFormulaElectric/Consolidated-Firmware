import { SignalType } from "@/lib/types/Signal";
import type { FC } from "react";
import { Color } from 'chroma-js';

// shared to all
type WidgetBase = {
  id: string;
  mock?: boolean; // TODO this might be the play?
}

// enum data
export interface EnumSignalConfig {
  signal_name: string;
  delay: number; // ms
  initialPoints: number;
  options: {
    colorPalette: Color[],
  };
  color: Color;
}
export type WidgetDataEnum = WidgetBase & {
  type: SignalType.ENUM;
  signals: Array<EnumSignalConfig>;
}

// numerical data
export interface NumericalSignalConfig {
  signal_name: string;
  delay: number; // ms
  color: Color;
  min: number;
  max: number;
}
export type WidgetDataNumerical = WidgetBase & {
  type: SignalType.NUMERICAL;
  signals: Array<NumericalSignalConfig>;
}

export type WidgetConfigs = EnumSignalConfig[] | NumericalSignalConfig[];
export type WidgetSignalConfig = EnumSignalConfig | NumericalSignalConfig;
export type WidgetData = WidgetDataNumerical | WidgetDataEnum;
export type WidgetRenderer = FC<WidgetData>;
