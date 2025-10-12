"use client";

import React, { createContext, useContext, useState } from 'react'
import type { Card } from '@/lib/types/DashboardCard';
import type { Widget, WIDGET_TYPE } from '@/lib/types/Widget';

type DashboardLayoutType = {
  cards: Card[]

  addCard: (newCard: Card) => void;
  removeCard: (cardToRemove: Card) => void;

  // NOTE(evan): Might cause performance issues with keeping widget state together with
  //             card state, if it becomes an issue move to per-card widget state.

  addWidget: (parentCard: Card, newWidget: Widget<WIDGET_TYPE>) => void;
  removeWidget: (parentCard: Card, widgetToRemove: Widget<WIDGET_TYPE>) => void;
}

const DashboardLayoutContext = createContext<DashboardLayoutType | undefined>(undefined)

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
            colorPalette: [
              "#FF637E",
              "#FFB86A",
              "#05DF72",
              "#51A2FF"
            ]
          }
        }
      ]
    }
  ]);

  // NOTE(evan): Card and widget lengths should never be super large so these
  //             array recreations are permitable when changing states.

  const addCard: DashboardLayoutType['addCard'] = React.useCallback((newCard) => {
    setCards((prev) => (
      [...prev, newCard]
    ))
  }, []);

  const removeCard: DashboardLayoutType['removeCard'] = React.useCallback((cardToRemove) => {
    setCards((prev) => (
      prev.toSpliced(prev.indexOf(cardToRemove), 1)
    ));
  }, []);

  const addWidget: DashboardLayoutType['addWidget'] = React.useCallback((parentCard, newWidget) => {
    setCards((prev) => {
      parentCard.widgets.push(newWidget);

      return [...prev];
    });
  }, []);

  const removeWidget: DashboardLayoutType['removeWidget'] = React.useCallback((parentCard, widgetToRemove) => {
    setCards((prev) => {
      parentCard.widgets.splice(parentCard.widgets.indexOf(widgetToRemove), 1);

      return [...prev];
    });
  }, []);

  return (
    <DashboardLayoutContext.Provider
        value={{
          cards,

          addCard,
          removeCard,

          addWidget,
          removeWidget
        }}
    >
      {children}
    </DashboardLayoutContext.Provider>
  )
}

const useDashboardLayout = () => {
  const context = useContext(DashboardLayoutContext);

  if (context === undefined) {
    throw new Error('useDashboardLayout must be used within a DashboardLayoutProvider')
  }

  return context
}

export {
    DashboardLayoutProvider,
    useDashboardLayout
};
