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
  if (variant === "rowSide") {
    return (
      <button onClick={onClick} className="">
        <Plus size={32} />
      </button>
    );
  }

  // insertionCenter
  return (
    <div onClick={onClick} className="">
      <Plus size={32} />
    </div>
  );
};
