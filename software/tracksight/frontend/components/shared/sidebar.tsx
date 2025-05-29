"use client"
import { Database, SaveIcon } from "lucide-react"
import type React from "react"

import { ThemeToggle } from "@/components/shared/theme-toggle"
import { PausePlayButton } from "@/components/shared/pause-play-control"
import Image from "next/image"

interface SidebarProps {
  activePage: string
  setActivePage: (page: string) => void
}

export default function Sidebar({ activePage, setActivePage }: SidebarProps) {
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
            onClick={() => setActivePage("live-data")}
          />
          <NavItem
            icon={<SaveIcon size={18} />}
            label="Logging"
            isActive={activePage === "logging"}
            onClick={() => setActivePage("logging")}
          />
        </ul>
      </nav>

      {/* Absolutely centered Pause/Play Button */}
      <div className="absolute left-1/2 top-1/2 transform -translate-x-1/2 -translate-y-1/2">
        <PausePlayButton />
      </div>

      {/* Theme toggle - pushed to right */}
      <div className="flex items-center ml-auto">
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
  const handleClick = () => {
    console.log(`Navigating to: ${label}`)
    onClick()
  }

  return (
    <li>
      <button
        onClick={handleClick}
        className={`flex items-center px-2 py-1 text-sm rounded-md transition-colors ${
          isActive ? "bg-blue-800 text-white" : "text-blue-100 hover:bg-blue-800/50"
        }`}
      >
        <span className="mr-2">{icon}</span>
        <span>{label}</span>
      </button>
    </li>
  )
}
