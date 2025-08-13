"use client"
import { Database, Trash2, ArrowRight } from "lucide-react"
import { usePathname, useRouter } from "next/navigation"
import type React from "react"

import { ThemeToggle } from "@/components/ui/theme-toggle"
import { PausePlayButton, useDisplayControl } from "@/components/shared/PausePlayControl"
import Image from "next/image"
import { useMemo } from "react"

export default function Sidebar() {
  const pathname = usePathname();
  const activePage = useMemo(() => {
    if (pathname === "/live-data") {
      return "live-data";
    } else if (pathname === "/visualize") {
      return "visualize";
    } else if (pathname === "/dashboard") {
      return "dashboard";
    } else {
      return "live-data"; // Default to live-data
    }
  }, [pathname]);

  const router = useRouter()
  // const { clearData } = useSignals()
  const { isAutoscrollEnabled, toggleAutoscroll } = useDisplayControl()

  const handleNavigation = (page: string, route: string) => {
    router.push(route)
  }

  const handlePruneData = () => {
    if (confirm("Are you sure you want to clear all stored data? This will keep your signal subscriptions but remove all historical data.")) {
      // clearData()
    }
  }

  return (
    <div className="sticky top-0 h-16 w-full bg-blue-900 dark:bg-blue-900 relative flex items-center px-4">
      {/* Logo */}
      <div className="flex items-center">
        <Image
          src="https://hebbkx1anhila5yf.public.blob.vercel-storage.com/logo-q2Gcff7viO5jsIWpX3GPfyV7M52bUX.jpeg"
          alt="UBC Formula Electric"
          width={32}
          height={32}
          className="rounded-md"
        />
      </div>

      {/* Navigation */}
      <nav className="ml-6">
        <ul className="flex space-x-6">
          <NavItem
            icon={<Database size={18} />}
            label="Live Data"
            isActive={activePage === "live-data"}
            onClick={() => handleNavigation("live-data", "/live-data")}
          />
          {/* Future navigation items can be added here:
          <NavItem
            icon={<SaveIcon size={18} />}
            label="Dashboard"
            isActive={activePage === "dashboard"}
            onClick={() => handleNavigation("dashboard", "/dashboard")}
          />
          <NavItem
            icon={<ChartIcon size={18} />}
            label="Visualize"
            isActive={activePage === "visualize"}
            onClick={() => handleNavigation("visualize", "/visualize")}
          /> */}
        </ul>
      </nav>

      {/* Absolutely centered Pause/Play Button */}
      <div className="absolute left-1/2 top-1/2 transform -translate-x-1/2 -translate-y-1/2">
        <PausePlayButton />
      </div>

      {/* Right side controls */}
      <div className="flex items-center ml-auto space-x-3">
        {/* Autoscroll Toggle Button */}
        <button
          onClick={toggleAutoscroll}
          className={`p-2 rounded-md transition-colors relative ${isAutoscrollEnabled
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
          className="p-2 rounded-md bg-red-600 text-white hover:bg-red-700 transition-colors"
          title="Clear all stored data (keeps subscriptions)"
          aria-label="Prune data"
        >
          <Trash2 size={18} />
        </button>

        {/* Theme Toggle */}
        <ThemeToggle />
      </div>
    </div>
  )
}

interface NavItemProps {
  icon: React.ReactNode
  label: string
  isActive: boolean
  onClick: () => void
}

function NavItem({ icon, label, isActive, onClick }: NavItemProps) {
  return (
    <li>
      <button
        onClick={onClick}
        className={`flex items-center px-2 py-1 text-sm rounded-md transition-colors ${isActive ? "bg-blue-800 text-white" : "text-blue-100 hover:bg-blue-800/50"
          }`}
      >
        <span className="mr-2">{icon}</span>
        <span>{label}</span>
      </button>
    </li>
  )
}
