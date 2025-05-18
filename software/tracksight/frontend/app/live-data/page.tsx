"use client"

import { SidebarProvider } from "@/components/ui/sidebar"
import Sidebar from "@/components/sidebar"
import { useTheme } from "next-themes"
import { useState, useEffect } from "react"
import EnumerationGraph from "./Enumeration"

export default function LiveDataPage() {
  const [activePage, setActivePage] = useState<string>("live-data")
  const { theme } = useTheme()
  const [mounted, setMounted] = useState(false)

  // Only render the UI after mounting to avoid hydration mismatch
  useEffect(() => {
    setMounted(true)
  }, [])

  if (!mounted) {
    return null
  }

  return (
    <SidebarProvider>
      <div className={`flex h-screen ${theme === "dark" ? "bg-black" : "bg-gray-100"}`}>
        <Sidebar activePage={activePage} setActivePage={setActivePage} />
        <main className="flex-1 overflow-auto">
          <div className="p-6 text-gray-800 dark:text-white">
            <h1 className="text-2xl font-bold mb-4">Live Data</h1>
            <EnumerationGraph currentTime={Date.now()} signalName="VC_State" />
          </div>
        </main>
      </div>
    </SidebarProvider>
  )
}
