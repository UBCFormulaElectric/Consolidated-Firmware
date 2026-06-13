"use client";

import { useEffect, useState } from "react";

import useListSDCards from "@/lib/hooks/useListSDCards";

import SDCardFileNavigator from "./SDCardFileNavigator";
import SDCardPicker from "./SDCardPicker";

type SDCardDumpPageProps = {
  initialSelectedSDCard?: string | null;
};

const SDCardDumpPage = (props: SDCardDumpPageProps) => {
  const { initialSelectedSDCard = null } = props;

  const [selectedSDCard, setSelectedSDCard] = useState<string | null>(initialSelectedSDCard);
  const { data: sdCards, isLoading, error } = useListSDCards();

  useEffect(() => {
    if (!sdCards || sdCards.length === 0) {
      return;
    }

    if (!selectedSDCard || !sdCards.includes(selectedSDCard)) {
      setSelectedSDCard(sdCards[0]);
    }
  }, [sdCards, selectedSDCard]);

  return (
    <>
      <SDCardPicker
        selected={selectedSDCard}
        onSelect={setSelectedSDCard}
        sdCards={sdCards}
        isLoading={isLoading}
        error={error}
      />
      <div className="flex-1 flex min-h-0">
        <SDCardFileNavigator sdCard={selectedSDCard} />
      </div>
    </>
  );
};

export default SDCardDumpPage;
