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

export type WidgetData = WidgetDataEnum | WidgetDataNumerical;
export type WidgetRenderer = FC<WidgetData>;