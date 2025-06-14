"use client";

import { Plus } from "lucide-react";
import React from "react";

export type PlusVariant = "insertionCenter" | "rowSide";

interface PlusButtonProps {
  onClick?: () => void;
  variant?: PlusVariant;
}

export const PlusButton: React.FC<PlusButtonProps> = ({
  onClick,
  variant = "insertionCenter",
}) => {
  return (
    <div
      onClick={onClick}
      className="border-green-600 bg-green-500 rounded-full text-white hover:scale-105 active:scale-95  hover:bg-green-600"
    >
      <Plus size={32} />
    </div>
  );
};
