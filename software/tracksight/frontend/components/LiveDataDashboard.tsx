"use client";

import React from "react";

import Card from "@/components/common/Card";
import Widget from "@/components/widgets/Widget";
import { useDashboardLayout } from "@/lib/contexts/DashboardLayout";
import { useEditMode } from "@/lib/contexts/EditModeContext";
import EditableDataDashboard from "./EditableDataDashboard";

const LiveDataDashboard: React.FC = () => {
  const { cards, addWidget, addCard } = useDashboardLayout();
  const { isEditMode } = useEditMode();

  if (isEditMode) {
    return <EditableDataDashboard />;
  }

  return (
    <div className="flex h-full w-full flex-col gap-4">
      {cards.map((card) => (
        <Card
          title={card.title || "Card"}
        >
          {card.widgets.map((widget) => (
            <Widget {...widget} />
          ))}
        </Card>
      ))}
    </div>
  );
};

export default LiveDataDashboard;
