"use client";

import React from "react";
import { PlusButton } from "./PlusButton";

interface InsertionBarProps {
  onInsert: () => void;
}

export const InsertionBar: React.FC<InsertionBarProps> = ({ onInsert }) => (
  <div
    onClick={onInsert}
    className="sticky inline-block w-[calc(100vw-48px)] left-2"
  >
    <div className="relative left-[calc(50vw-39px)] inline-block">
      <PlusButton />
    </div>
  </div>
);
