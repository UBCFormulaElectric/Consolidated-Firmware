"use client";

import type { Card } from "@/lib/types/DashboardCard";
import type { Widget, WIDGET_TYPE } from "@/lib/types/Widget";
import React, { createContext, useContext, useState } from "react";

type DashboardLayoutType = {
  cards: Card[];

  addCard: (newCard: Card) => void;
  removeCard: (cardToRemove: Card) => void;
  editCard: (cardToEdit: Card, newCardData: Partial<Card>) => void;

  // NOTE(evan): Might cause performance issues with keeping widget state together with
  //             card state, if it becomes an issue move to per-card widget state.

  addWidget: (parentCard: Card, newWidget: Widget<WIDGET_TYPE>) => void;
  removeWidget: (parentCard: Card, widgetToRemove: Widget<WIDGET_TYPE>) => void;
  editWidget: (
    parentCard: Card,
    widgetToEdit: Widget<WIDGET_TYPE>,
    newWidgetData: Partial<Widget<WIDGET_TYPE>>
  ) => void;
};

const DashboardLayoutContext = createContext<DashboardLayoutType | undefined>(undefined);

const DashboardLayoutProvider = ({ children }: { children: React.ReactNode }) => {
  const [cards, setCards] = useState<Card[]>([
    // TODO(evan): Remove this it's just temporary data, should be pulled from local storage
    //             or something similar
    {
      title: "Battery Information",
      widgets: [
        {
          type: "stateTimeline",
          signals: ["VC_State"],
          options: {
            colorPalette: ["#FF637E", "#FFB86A", "#05DF72", "#51A2FF"],
          },
        },
      ],
    },
  ]);

  // NOTE(evan): Card and widget lengths should never be super large so these
  //             array recreations are permitable when changing states.

  const addCard: DashboardLayoutType["addCard"] = React.useCallback((newCard) => {
    setCards((prev) => [...prev, newCard]);
  }, []);

  const removeCard: DashboardLayoutType["removeCard"] = React.useCallback((cardToRemove) => {
    setCards((prev) => prev.toSpliced(prev.indexOf(cardToRemove), 1));
  }, []);

  const editCard: DashboardLayoutType["editCard"] = React.useCallback((cardToEdit, newCardData) => {
    setCards((prev) => {
      const cardIndex = prev.indexOf(cardToEdit);

      if (cardIndex === -1) {
        console.warn("Card to edit not found");
        return prev;
      }

      const updatedCard = {
        ...prev[cardIndex],
        ...newCardData,
      };

      const newCards = [...prev];
      newCards[cardIndex] = updatedCard;

      return newCards;
    });
  }, []);

  const addWidget: DashboardLayoutType["addWidget"] = React.useCallback((parentCard, newWidget) => {
    setCards((prev) => {
      parentCard.widgets.push(newWidget);

      return [...prev];
    });
  }, []);

  const removeWidget: DashboardLayoutType["removeWidget"] = React.useCallback(
    (parentCard, widgetToRemove) => {
      setCards((prev) => {
        parentCard.widgets.splice(parentCard.widgets.indexOf(widgetToRemove), 1);

        return [...prev];
      });
    },
    []
  );

  const editWidget: DashboardLayoutType["editWidget"] = React.useCallback(
    (parentCard, widgetToEdit, newWidgetData) => {
      setCards((prev) => {
        const widgetIndex = parentCard.widgets.indexOf(widgetToEdit);

        if (widgetIndex === -1) {
          console.warn("Widget to edit not found in parent card");
          return prev;
        }

        parentCard.widgets[widgetIndex] = {
          ...parentCard.widgets[widgetIndex],
          ...newWidgetData,
        };

        return [...prev];
      });
    },
    []
  );

  return (
    <DashboardLayoutContext.Provider
      value={{
        cards,

        addCard,
        removeCard,
        editCard,

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
