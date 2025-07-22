import type React from "react"
import type { Metadata } from "next"
import { Inter } from "next/font/google"
import './globals.css'
import { ThemeProvider } from "@/components/shared/ThemeProvider"
import { SignalProvider } from "@/lib/contexts/SignalContext"
import { PausePlayProvider } from "@/components/shared/PausePlayControl"
import LayoutWrapper from "@/components/shared/LayoutWrapper"

const inter = Inter({ subsets: ["latin"] })

export const metadata: Metadata = {
  title: "Tracksight Dashboard",
  description: "Monitoring and visualization dashboard for system data",
  generator: 'v0.dev'
}

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode
}>) {
  return (
    <html lang="en" suppressHydrationWarning>
      <body className={inter.className}>
        <ThemeProvider attribute="class" defaultTheme="dark" enableSystem>
          <PausePlayProvider>
            <SignalProvider>
              <LayoutWrapper>
                {children}
              </LayoutWrapper>
            </SignalProvider>
          </PausePlayProvider>
        </ThemeProvider>
      </body>
    </html>
  )
}
