import { SignalType } from "@/lib/types/Signal";
import type { FC } from "react";

type WidgetDataBase = {
  id: string;
}

interface enumProps {
  type: SignalType.ENUM;
  signal: string | null;
  options: {
    colorPalette: string[],
  };
}
export type WidgetDataEnum = WidgetDataBase & enumProps;

interface numericalProps {
  type: SignalType.NUMERICAL;
  signals: string[];
}
export type WidgetDataNumerical = WidgetDataBase & numericalProps;

export interface MockGraphConfig {
  type: "numerical" | "enumeration";
  delay: number; // ms
  initialPoints: number;
  signalName: string;
}

interface mockProps {
    type: SignalType.MOCK;
    configs: MockGraphConfig[];
}
export type WidgetDataMock = WidgetDataBase & mockProps;

export type WidgetData = WidgetDataEnum | WidgetDataNumerical | WidgetDataMock;
export type WidgetRenderer = FC<WidgetData>;
