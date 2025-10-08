"use client";

import { Pencil, File } from "lucide-react"

import { useEditMode } from "@/lib/contexts/EditModeContext";

const EditModeToggle = () => {
  const { isEditMode, toggleEditMode } = useEditMode();

  const Icon = isEditMode ? File : Pencil;

  return (
    <div className="hover:cursor-pointer">
      <Icon
        className="stroke-current hover:text-primary-900"
        onClick={toggleEditMode}
        size={24}
      />
    </div>
  )
}

export default EditModeToggle;
