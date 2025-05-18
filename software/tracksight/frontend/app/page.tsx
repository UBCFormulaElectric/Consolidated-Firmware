"use client"

import { useState, useEffect } from "react"
import { SidebarProvider } from "@/components/ui/sidebar"
import Dashboard from "@/components/dashboard"
import Sidebar from "@/components/sidebar"
import { useTheme } from "next-themes"

export default function Home() {
  const [activePage, setActivePage] = useState<string>("dashboard")
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
          <Dashboard activePage={activePage} />
        </main>
      </div>
    </SidebarProvider>
  )
}
