"use client";

import React, { useCallback } from "react";

import type { Card as CardData } from "@/lib/types/DashboardCard";
import type { WIDGET_TYPE, Widget as WidgetData } from "@/lib/types/Widget";

import Card from "@/components/common/Card";
import Widget from "@/components/widgets/Widget";
import { useDashboardLayout } from "@/lib/contexts/DashboardLayout";
import EditableText from "./EditableText";

const AddWidgetButton: React.FC<{ parentCard: CardData }> = (props) => {
  const { parentCard } = props;

  const { addWidget } = useDashboardLayout();

  return (
    <button
      className="mt-2 rounded bg-green-400 px-4 py-2 text-white hover:bg-green-500 hover:cursor-pointer"
      // TODO(evan): Make this a widget picker (still gotta design that)
      onClick={() => {
        addWidget(parentCard, {
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
  );
}

const AddCardButton: React.FC = () => {
  const { addCard } = useDashboardLayout();

  return (
    <button
      className="mt-2 rounded bg-green-400 px-4 py-2 text-white hover:bg-green-500 hover:cursor-pointer"
      onClick={() => {
        addCard({
          title: "",
          widgets: [],
        });
      }}
    >
      Add Card
    </button>
  );
}

const EditableDataDashboard: React.FC = () => {
  const { cards, editWidget, addWidget, editCard } = useDashboardLayout();

  const createEditWidgetFunction = useCallback(
    (parent: CardData, widget: WidgetData<WIDGET_TYPE>) => {
      return (newWidgetData: Partial<WidgetData<WIDGET_TYPE>>) => {
        editWidget(parent, widget, newWidgetData);
      };
    },
    [editWidget]
  );

  return (
    <div className="flex h-full w-full flex-col gap-4">
      {cards.map((card) => (
        <Card
          title={
            <EditableText
              onChange={(newText) => editCard(card, { title: newText.trim() })}
              placeholder="Card"
              initialText={card.title}
            />
          }
        >
          {card.widgets.map((widget) => (
            <Widget {...widget} editWidget={createEditWidgetFunction(card, widget)} />
          ))}

          <div className="w-full flex justify-center py-5">
            <AddWidgetButton parentCard={card} />
          </div>
        </Card>
      ))}

      <div className="w-full flex justify-center py-5">
        <AddCardButton />
      </div>
    </div>
  );
};

export default EditableDataDashboard;
