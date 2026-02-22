"use client";

import { Plus } from "lucide-react";
import { HTMLAttributes } from "react";

export function PlusButton(props: HTMLAttributes<HTMLDivElement>) {
  return (
    <div
      {...props}
      className="border-green-600 bg-green-500 rounded-full text-white hover:scale-105 active:scale-95  hover:bg-green-600 cursor-pointer transition-transform duration-150 w-min"
    >
      <Plus size={32} />
    </div>
  );
};
