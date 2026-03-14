import { createContext, useCallback, useContext, useMemo, ReactNode } from "react";

import { v4 as uuidv4 } from 'uuid';

import chroma from "chroma-js";

import { IS_DEBUG } from "@/lib/constants";

import { useLocalState } from "@/lib/hooks/useLocalState";
import { WidgetData, WidgetType } from "@/lib/types/Widget";

const LOCAL_STORAGE_KEY = "tracksight_widgets_config_v1";

interface WidgetManagerContext {
  widgets: WidgetData[];
  initializedFromLocalStorage: boolean;
  appendWidget: (newWidget: WidgetData) => void;
  removeWidget: (widgetToRemove: string) => void;
  // setEnumSignal: (widgetID: string, newSignal: string) => void;
  appendSignal: <T extends WidgetType, Widget extends Extract<WidgetData, { type: T }>>(widget: Widget, newSignal: Widget["signals"][number]) => void;
  removeSignal: (widget: WidgetData, nameOfSignalToRemvoe: string) => void;
  updateWidget: <T extends WidgetType, Widget extends Extract<WidgetData, { type: T }>>(widget: Widget, updater: (prevWidget: Widget) => Widget) => void;
}

const WidgetManagerContext = createContext<WidgetManagerContext | null>(null);

export function useWidgetManager() {
  const ctx = useContext(WidgetManagerContext);
  if (!ctx) { throw new Error("useWidgetManagerContext must be used within a WidgetManagerProvider"); }
  return ctx;
}

function WidgetSerialize(widgets: WidgetData[]): string {
  return JSON.stringify(widgets.map(w => {
    if (w.type == "enumTimeline") {
      return {
        ...w,
        options: {
          ...w.options,
          colorPalette: Object.fromEntries(Object.entries(w.options.colorPalette).map(([signalName, enumColors]) => [
            signalName,
            {
              color: enumColors.color.hex(),
              enumValueColors: Object.fromEntries(Object.entries(enumColors.enumValueColors).map(([enumVal, color]) => [
                enumVal,
                color.hex()
              ]))
            }
          ]))
        }
      }
    }
    else if (w.type == "numericalGraph") {
      return {
        ...w,
        options: {
          ...w.options,
          colorPalette: Object.fromEntries(Object.entries(w.options.colorPalette).map(([signalName, color]) => [
            signalName,
            color.hex()
          ])),
        }
      }
    }
  }));
}

// yolo if it doesn't have it it is what it is fr
function WidgetDeserialize(widgetString: string): WidgetData[] {
  const back = JSON.parse(widgetString) as unknown & WidgetData[];

  if (!Array.isArray(back)) {
    throw new Error("Deserialized widget config is not an array");
  }

  return back.map(b => {
    if (b.type === "enumTimeline") {
      return {
        id: b.id,
        type: "enumTimeline",
        data: b.data,
        signals: b.signals,
        options: {
          height: b.options.height,
          timeTickCount: b.options.timeTickCount,
          colorPalette: Object.keys(b.options.colorPalette).reduce((acc, signalName) => ({
            ...acc,
            [signalName]: Object.keys(b.options.colorPalette[signalName]).reduce((enumAcc, enumVal) => ({
              ...enumAcc,
              [enumVal]: {
                color: chroma(b.options.colorPalette[signalName].color),
                enumValueColors: Object.fromEntries(Object.entries(b.options.colorPalette[signalName].enumValueColors).map(([enumVal, color]) => [
                  enumVal,
                  chroma(color)
                ]))
              }
            }), {})
          }), {})
        }
      }
    }
    else if (b.type === "numericalGraph") {
      return {
        id: b.id,
        type: "numericalGraph",
        signals: b.signals,
        data: b.data,
        options: {
          colorPalette: Object.fromEntries(Object.entries(b.options.colorPalette).map(([signalName, color]) => [
            signalName,
            chroma(color)
          ])),
          height: b.options.height,
          timeTickCount: b.options.timeTickCount,
        }
      }
    }
    else {
      throw new Error("Deserialized widget has invalid type: " + (b as any).type);
    }
  })
}

export function WidgetManager({ children }: { children: ReactNode }) {
  const [widgets, setWidgets, isInitialized] = useLocalState<WidgetData[]>(LOCAL_STORAGE_KEY, [], WidgetSerialize, WidgetDeserialize);

  const appendWidget = useCallback((newWidget: WidgetData) => {
    newWidget.id = uuidv4();
    setWidgets((prev) => [...prev, newWidget]);
  }, []);

  const removeWidget = useCallback((widgetToRemove: string) => {
    setWidgets((prev) => {
      const widgetIndex = prev.findIndex((w) => w.id === widgetToRemove);
      if (widgetIndex === -1) {
        IS_DEBUG && console.warn("Widget to remove not found");
        return prev;
      }
      const newWidgets = [...prev];
      newWidgets.splice(widgetIndex, 1);
      return newWidgets;
    });
  }, []);

  const appendSignal = useCallback(<T extends WidgetType, Widget extends Extract<WidgetData, { type: T }>>(widget: Widget, newSignal: Widget["signals"][number]) => {
    setWidgets((prev) => {
      const newWidgets = [...prev];
      let updateWidget = newWidgets.find((w) => w.id === widget.id) as Widget;

      if (!updateWidget) {
        IS_DEBUG && console.warn("Widget to edit not found");
        return prev;
      }

      if (updateWidget.signals.some(c => c.name === newSignal.name)) {
        IS_DEBUG && console.warn("Signal already exists in widget: " + newSignal.name);
        return prev;
      }

      updateWidget = {
        ...updateWidget,
        signals: [...updateWidget.signals, newSignal],
      };

      return newWidgets;
    });
  }, []);

  const removeSignal = useCallback((widget: WidgetData, nameOfSignalToRemove: string) => {
    setWidgets((prev) => {
      const newWidgets = [...prev];

      let updateWidget = newWidgets.find((w) => w.id === widget.id);

      if (!updateWidget) {
        IS_DEBUG && console.warn("Widget to edit not found");
        return prev;
      }

      updateWidget = {
        ...updateWidget,
        signals: updateWidget.signals.filter(c => c.name !== nameOfSignalToRemove) as Array<any> // trust
      };

      return newWidgets;
    });
  }, []);

  const updateWidget = useCallback(<T extends WidgetType, Widget extends Extract<WidgetData, { type: T }>>(widget: Widget, updater: (prevWidget: Widget) => Widget) => {
    setWidgets((prev) => {
      const updateWidget = updater(widget);

      return [
        ...prev.filter((w) => w.id !== widget.id),
        updateWidget
      ]
    });
  }, []);

  const CTXVAL = useMemo<WidgetManagerContext>(() => ({
    widgets, appendWidget, removeWidget,
    appendSignal, removeSignal,
    updateWidget,
    initializedFromLocalStorage: isInitialized
  }), [
    widgets, appendWidget, removeWidget,
    appendSignal, removeSignal,
    updateWidget, isInitialized
  ]);

  return (
    <WidgetManagerContext value={CTXVAL} >
      {children}
    </WidgetManagerContext>
  )
}
