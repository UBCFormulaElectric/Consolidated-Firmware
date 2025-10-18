"use client";

import React, { useCallback } from "react";

import type { Card as CardData } from "@/lib/types/DashboardCard";
import type { WIDGET_TYPE, Widget as WidgetData } from "@/lib/types/Widget";

import Card from "@/components/common/Card";
import Widget from "@/components/widgets/Widget";
import { useDashboardLayout } from "@/lib/contexts/DashboardLayout";
import { useEditMode } from "@/lib/contexts/EditModeContext";

const LiveDataDashboard: React.FC = () => {
  const { cards, editWidget, addWidget } = useDashboardLayout();
  const { isEditMode } = useEditMode();

  const createEditWidgetFunction = useCallback(
    (parent: CardData, widget: WidgetData<WIDGET_TYPE>) => {
      return (newWidgetData: Partial<WidgetData<WIDGET_TYPE>>) => {
        editWidget(parent, widget, newWidgetData);
      };
    },
    [editWidget]
  );

  return (
    <div className="flex h-full w-full flex-col">
      {cards.map((card) => (
        <Card title={card.title}>
          {card.widgets.map((widget) => (
            <Widget {...widget} editWidget={createEditWidgetFunction(card, widget)} />
          ))}
          {
            isEditMode && (
              <div className="w-full flex justify-center py-5">
                <button
                  className="mt-2 rounded bg-green-400 px-4 py-2 text-white hover:bg-green-500 hover:cursor-pointer"
                  onClick={() => {
                    addWidget(card, {
                      type: "stateTimeline",
                      signals: [],
                      options: {
                        colorPalette: ["#3b82f6", "#10b981", "#f59e0b", "#ef4444"],
                      },
                    })
                  }}
                >
                  Add Widget
                </button>
              </div>
            )
          }
        </Card>
      ))}
    </div>
  );
};

export default LiveDataDashboard;
