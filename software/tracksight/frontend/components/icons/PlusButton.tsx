"use client";

import { Plus } from "lucide-react";
import { ComponentProps } from "react";

import { cn } from "@/lib/utils";
import { CircleIconButton } from "./CircleIconButton";

export function PlusButton(props: ComponentProps<"button">) {
    const { className, ...buttonProps } = props;

    return <CircleIconButton {...buttonProps} Icon={Plus} className={cn("bg-green-500 hover:bg-green-600", className)} />;
}
