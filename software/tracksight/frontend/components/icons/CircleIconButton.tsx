"use client";

import { LucideIcon } from "lucide-react";
import { HTMLAttributes } from "react";

const BUTTON_ICON_SIZE_PX = 16;
const BUTTON_ICON_STROKE_WIDTH_PX = 1.75;

export function CircleIconButton(props: HTMLAttributes<HTMLDivElement> & { Icon: LucideIcon; colorClassName: string }) {
  const { Icon, colorClassName, ...divProps } = props;

  return (
    <div
      {...divProps}
      className={`flex size-8 shrink-0 items-center justify-center rounded-full text-white hover:scale-105 active:scale-95 cursor-pointer transition-transform duration-150 ${colorClassName}`}
    >
      <Icon size={BUTTON_ICON_SIZE_PX} strokeWidth={BUTTON_ICON_STROKE_WIDTH_PX} absoluteStrokeWidth />
    </div>
  );
}
