import './globals.css'

import type React from "react"
import type { Metadata } from "next"

import { Inter } from "next/font/google"
const inter = Inter({ subsets: ["latin"] })

import Sidebar from "@/components/shared/sidebar"
import Providers from "./providers"

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
      <body className={inter.className}>
        <Providers>
          <Sidebar />
          <main className="min-h-screen bg-gray-100 dark:bg-gray-900">{children}</main>
        </Providers>
      </body>
    </html>
  )
}
