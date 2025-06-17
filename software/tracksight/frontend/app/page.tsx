"use client"

import { useState } from "react"
import Sidebar from "@/components/shared/sidebar"
import LiveDataPage from "@/components/live-data/live-data-page"

export default function Home() {
  const [activePage, setActivePage] = useState<string>("live-data")

  // Render the appropriate page based on activePage state
  const renderActivePage = () => {
    switch (activePage) {
      case "live-data":
        return <LiveDataPage />
      default:
        return <LiveDataPage />
    }
  }

  return (
    <div className="flex flex-col h-screen bg-gray-100 dark:bg-gray-900">
      <Sidebar activePage={activePage} setActivePage={setActivePage} />
      <main className="flex-1 overflow-auto">{renderActivePage()}</main>
    </div>
  )
}
