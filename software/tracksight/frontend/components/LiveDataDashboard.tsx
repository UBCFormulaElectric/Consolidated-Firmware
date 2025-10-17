"use client";

import { useCallback } from "react";

import type { WIDGET_TYPE, Widget as WidgetData } from "@/lib/types/Widget";
import type { Card as CardData } from "@/lib/types/DashboardCard";

import { useDashboardLayout } from "@/lib/contexts/DashboardLayout";
import Card from "@/components/common/Card";
import Widget from "@/components/widgets/Widget";

const LiveDataDashboard: React.FC = () => {
  const { cards, editWidget } = useDashboardLayout();

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
        </Card>
      ))}
    </div>
  );
};

export default LiveDataDashboard;
