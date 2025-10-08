import './globals.css'

import type React from "react"
import type { Metadata } from "next"

import { Inter } from "next/font/google"
const inter = Inter({ subsets: ["latin"] })

import Sidebar from "@/components/Navbar"

export const metadata: Metadata = {
  title: "Tracksight Dashboard",
  description: "Monitoring and visualization dashboard for system data",
}

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode
}>) {
  return (
    <html lang="en" suppressHydrationWarning>
      <body className={`${inter.className} bg-primary-200`}>
        <Sidebar />
        <main className="mt-24">{children}</main>
      </body>
    </html>
  )
}
