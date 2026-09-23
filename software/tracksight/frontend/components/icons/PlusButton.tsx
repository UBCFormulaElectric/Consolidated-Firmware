"use client";

import { Plus } from "lucide-react";
import { HTMLAttributes } from "react";

import { CircleIconButton } from "./CircleIconButton";

export function PlusButton(props: HTMLAttributes<HTMLDivElement>) {
  return <CircleIconButton {...props} Icon={Plus} colorClassName="bg-green-500 hover:bg-green-600" />;
};
