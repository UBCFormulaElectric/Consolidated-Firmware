"use client"
import { Home, FileText, Database, Settings, SaveIcon } from "lucide-react"
import type React from "react"

import { ThemeToggle } from "@/components/shared/theme-toggle"
import Image from "next/image"

interface SidebarProps {
  activePage: string
  setActivePage: (page: string) => void
}

export default function Sidebar({ activePage, setActivePage }: SidebarProps) {
  return (
    <div className="w-56 h-screen bg-blue-900 dark:bg-blue-900 flex flex-col">
      {/* Logo */}
      <div className="flex justify-center py-4">
        <Image
          src="https://hebbkx1anhila5yf.public.blob.vercel-storage.com/logo-q2Gcff7viO5jsIWpX3GPfyV7M52bUX.jpeg"
          alt="UBC Formula Electric"
          width={120}
          height={120}
          className="rounded-md"
        />
      </div>

      {/* Navigation */}
      <nav className="flex-1 px-3 py-2">
        <ul className="space-y-1">
          <NavItem
            icon={<Home size={18} />}
            label="Dashboard"
            isActive={activePage === "dashboard"}
            onClick={() => setActivePage("dashboard")}
          />
          <NavItem
            icon={<FileText size={18} />}
            label="User Guide"
            isActive={activePage === "user-guide"}
            onClick={() => setActivePage("user-guide")}
          />
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
          <NavItem
            icon={<Settings size={18} />}
            label="Settings"
            isActive={activePage === "settings"}
            onClick={() => setActivePage("settings")}
          />
        </ul>
      </nav>

      {/* Footer with theme toggle */}
      <div className="p-3 border-t border-blue-800 flex justify-center">
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
        className={`flex items-center w-full px-3 py-2 text-sm rounded-md transition-colors ${
          isActive ? "bg-blue-800 text-white" : "text-blue-100 hover:bg-blue-800/50"
        }`}
      >
        <span className="mr-2">{icon}</span>
        <span>{label}</span>
      </button>
    </li>
  )
}
