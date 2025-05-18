"use client"

import { ReactNode } from "react"

export default function TestingLayout({
  children,
}: {
  children: ReactNode
}) {
  return (
    <div className="container mx-auto px-4 py-8">
      <h1 className="text-3xl font-bold mb-6">Testing Dashboard</h1>
      {children}
    </div>
  )
}
