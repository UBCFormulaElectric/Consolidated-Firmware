"use client";

import { Pencil } from "lucide-react";
import { HTMLAttributes } from "react";

import { CircleIconButton } from "./CircleIconButton";

export function EditButton(props: HTMLAttributes<HTMLDivElement>) {
  return <CircleIconButton {...props} Icon={Pencil} colorClassName="bg-gray-400 hover:bg-gray-500" />;
};
