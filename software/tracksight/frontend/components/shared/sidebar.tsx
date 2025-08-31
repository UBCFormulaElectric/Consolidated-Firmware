"use client"
import { useCallback } from "react"
import Image from "next/image"
import Link from "next/link"
import { usePathname } from "next/navigation"
import { Database, Trash2, ArrowRight } from "lucide-react"

import { ThemeToggle } from "@/components/ui/theme-toggle"
import { PausePlayButton, useDisplayControl } from "@/components/shared/PausePlayControl"

export default function Sidebar() {
  const { isAutoscrollEnabled, toggleAutoscroll } = useDisplayControl()
  const handlePruneData = useCallback(() => {
    if (confirm("Are you sure you want to clear all stored data? This will keep your signal subscriptions but remove all historical data.")) {
    }
  }, [])
  return (
    <nav className="fixed top-0 h-16 left-0 right-0 bg-blue-900 dark:bg-blue-900 flex items-center px-4">
      {/* Logo */}
      <div className="flex items-center">
        <Image
          src="https://hebbkx1anhila5yf.public.blob.vercel-storage.com/logo-q2Gcff7viO5jsIWpX3GPfyV7M52bUX.jpeg"
          alt="UBC Formula Electric"
          width={32} height={32} className="rounded-md"
        />
      </div>

      {/* Navigation */}
      <div className="ml-6">
        <NavItem icon={<Database size={18} />} label="Live Data" pathName="/live-data" />
        {/* Future navigation items can be added here:
          <NavItem
            icon={<SaveIcon size={18} />}
            label="Dashboard"
            pathName="/dashboard"
          />
          <NavItem
            icon={<ChartIcon size={18} />}
            label="Visualize"
            pathName="/visualize"
          /> */}
      </div>

      {/* Absolutely centered Pause/Play Button */}
      <div className="absolute left-1/2 top-1/2 transform -translate-x-1/2 -translate-y-1/2">
        <PausePlayButton />
      </div>

      {/* Right side controls */}
      <div className="flex items-center ml-auto space-x-3">
        {/* Autoscroll Toggle Button */}
        <button
          onClick={toggleAutoscroll}
          className={`p-2 rounded-md transition-colors relative cursor-pointer ${isAutoscrollEnabled
            ? "bg-green-600 text-white hover:bg-green-700"
            : "bg-gray-600 text-white hover:bg-gray-700"
            }`}
          title={isAutoscrollEnabled ? "Disable autoscroll" : "Enable autoscroll - automatically scroll to new data"}
          aria-label="Toggle autoscroll"
        >
          <ArrowRight size={18} className={isAutoscrollEnabled ? "animate-pulse" : ""} />
          {isAutoscrollEnabled && (
            <span className="absolute -top-1 -right-1 w-2 h-2 bg-green-400 rounded-full"></span>
          )}
        </button>

        {/* Prune Data Button */}
        <button
          onClick={handlePruneData}
          className="p-2 rounded-md bg-red-600 text-white hover:bg-red-700 transition-colors cursor-pointer"
          title="Clear all stored data (keeps subscriptions)"
          aria-label="Prune data"
        >
          <Trash2 size={18} />
        </button>

        {/* Theme Toggle */}
        <ThemeToggle />
      </div>
    </nav>
  )
}

function NavItem({ icon, label, pathName }: {
  icon: React.ReactNode
  label: string
  pathName: string
}) {
  const pathname = usePathname();
  return (
    <Link href={pathName}>
      <button className={`flex items-center px-2 py-1 text-sm rounded-md transition-colors cursor-pointer ${pathname == pathName ? "bg-blue-800 text-white" : "text-blue-100 hover:bg-blue-800/50"}`}>
        <span className="mr-2">{icon}</span>
        <span>{label}</span>
      </button>
    </Link>
  )
}
