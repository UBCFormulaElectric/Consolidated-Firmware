"use client";

import React from "react";
import { PlusButton } from "./PlusButton";

interface InsertionBarProps {
  onInsert: () => void;
}

export const InsertionBar: React.FC<InsertionBarProps> = ({ onInsert }) => (
  <div onClick={onInsert} className="sticky inline-block left-[50vw]">
    <div className="relative inline-block">
      <PlusButton />
    </div>
  </div>
);
