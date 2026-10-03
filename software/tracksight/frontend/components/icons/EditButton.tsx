"use client";

import { Pencil } from "lucide-react";
import { ComponentProps } from "react";

import { cn } from "@/lib/utils";
import { CircleIconButton } from "./CircleIconButton";

export function EditButton(props: ComponentProps<"button">) {
    const { className, ...buttonProps } = props;

    return <CircleIconButton {...buttonProps} Icon={Pencil} className={cn("bg-gray-400 hover:bg-gray-500", className)} />;
}
