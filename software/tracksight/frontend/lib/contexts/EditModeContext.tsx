"use client";

import React, { createContext, useContext, useState } from 'react'

type EditModeContextType = {
  isEditMode: boolean
  toggleEditMode: () => void
}

const EditModeContext = createContext<EditModeContextType | undefined>(undefined)

const EditModeProvider = ({ children }: { children: React.ReactNode }) => {
  const [isEditMode, setIsEditMode] = useState(false)

  const toggleEditMode = () => {
    setIsEditMode((prev) => (!prev))
  }

  return (
    <EditModeContext.Provider
      value={{
        isEditMode,
        toggleEditMode
      }}
    >
      {children}
    </EditModeContext.Provider>
  )
}

const useEditMode = () => {
  const context = useContext(EditModeContext);

  if (context === undefined) {
    throw new Error('useEditMode must be used within a EditModeProvider')
  }

  return context
}

export {
    EditModeProvider,
    useEditMode
};
