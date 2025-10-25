"use client";

import type { Widget, WIDGET_TYPE } from "@/lib/types/Widget";
import React, { createContext, ReactNode, useCallback, useContext, useState } from "react";
import { IS_DEBUG } from "@/lib/constants";

type DashboardLayoutType = {
  // NOTE(evan): Might cause performance issues with keeping widget state together with
  //             card state, if it becomes an issue move to per-card widget state.
  widgets: Widget<WIDGET_TYPE>[];

  addWidget: (newWidget: Widget<WIDGET_TYPE>) => void;
  removeWidget: (widgetToRemove: string) => void;
  editWidget: (
    widgetToEdit: string,
    newWidgetData: Partial<Widget<WIDGET_TYPE>>
  ) => void;
};

const DashboardLayoutContext = createContext<DashboardLayoutType | undefined>(undefined);

const DashboardLayoutProvider = ({ children }: { children: ReactNode }) => {
  const [widgets, setWidgets] = useState<Widget<WIDGET_TYPE>[]>([
    // TODO(evan): Remove this it's just temporary data, should be pulled from local storage
    //             or something similar
    {
      type: "stateTimeline",
      signals: ["VC_State"],
      options: {
        colorPalette: ["#FF637E", "#FFB86A", "#05DF72", "#51A2FF"],
      },
      id: "widget-1",
    },
    {
      type: "stateTimeline",
      signals: ["VC_State"],
      options: {
        colorPalette: ["#FF637E", "#FFB86A", "#05DF72", "#51A2FF"],
      },
      id: "widget-2",
    },
    {
      type: "stateTimeline",
      signals: ["VC_State"],
      options: {
        colorPalette: ["#FF637E", "#FFB86A", "#05DF72", "#51A2FF"],
      },
      id: "widget-3",
    },
    {
      type: "stateTimeline",
      signals: ["VC_State"],
      options: {
        colorPalette: ["#FF637E", "#FFB86A", "#05DF72", "#51A2FF"],
      },
      id: "widget-4",
    },
  ]);

  const addWidget: DashboardLayoutType["addWidget"] = useCallback((newWidget) => {
    newWidget.id = crypto.randomUUID();

    setWidgets((prev) => [...prev, newWidget]);
  }, []);

  const removeWidget: DashboardLayoutType["removeWidget"] = useCallback(
    (widgetToRemove) => {
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
    },
    []
  );

  const editWidget: DashboardLayoutType["editWidget"] = useCallback(
    (widgetID, newWidgetData) => {
      setWidgets((prev) => {
        const widgetIndex = prev.findIndex((w) => w.id === widgetID);

        if (widgetIndex === -1) {
          IS_DEBUG && console.warn("Widget to edit not found");

          return prev;
        }

        const newWidgets = [...prev];
        newWidgets[widgetIndex] = {
          ...newWidgets[widgetIndex],
          ...newWidgetData,
        };

        return newWidgets;
      });
    },
    []
  );

  return (
    <DashboardLayoutContext.Provider
      value={{
        widgets,


        addWidget,
        removeWidget,
        editWidget,
      }}
    >
      {children}
    </DashboardLayoutContext.Provider>
  );
};

const useDashboardLayout = () => {
  const context = useContext(DashboardLayoutContext);

  if (context === undefined) {
    throw new Error("useDashboardLayout must be used within a DashboardLayoutProvider");
  }

  return context;
};

export { DashboardLayoutProvider, useDashboardLayout };
