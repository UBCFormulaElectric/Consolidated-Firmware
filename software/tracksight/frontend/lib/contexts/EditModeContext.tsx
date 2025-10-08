'use client'

import React, { createContext, useContext, useState } from 'react'

interface EditModeContextType {
  isEditMode: boolean
  toggleEditMode: () => void
}

const EditModeContext = createContext<EditModeContextType | undefined>(undefined)

export function EditModeProvider({ children }: { children: React.ReactNode }) {
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

export function useEditMode() {
  const context = useContext(EditModeContext);

  if (context === undefined) {
    throw new Error('useEditMode must be used within a EditModeProvider')
  }

  return context
}