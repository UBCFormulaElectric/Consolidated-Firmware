"use client"

import { Calendar, Pencil } from "lucide-react"

import { PausePlayButton } from "@/components/shared/PausePlayControl"

{/** TODO(eroxl): Add the dark mode colours to this */ }
export default function Sidebar() {
  return (
    <nav className="fixed top-0 h-min px-10 py-5 left-0 bg-gray-200 text-gray-800 w-screen flex flex-row justify-between items-center select-none">
      {/** TODO(eroxl): Make this dynamic and a selector **/}
      <div className="flex gap-4 items-center hover:cursor-pointer hover:text-black transition-colors px-2 py-1 rounded-md">
        <Calendar className="stroke-current" size={24} />
        <div className="flex flex-col items-start">
          <span className="text font-semibold">
            September 28th 2025
          </span>
          <span className="opacity-50 text-sm">
            Session 1
          </span>
        </div>
      </div>

      <div className="absolute left-1/2 top-1/2 transform -translate-x-1/2 -translate-y-1/2">
        <PausePlayButton />
      </div>

      <div>
        <Pencil className="stroke-current hover:text-black hover:cursor-pointer" size={24} />
      </div>
    </nav>
  )
}

