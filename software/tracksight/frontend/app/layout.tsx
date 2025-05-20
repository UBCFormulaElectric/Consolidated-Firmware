import type React from "react"
import type { Metadata } from "next"
import { Inter } from "next/font/google"
import "./globals.css"
import { ThemeProvider } from "@/components/shared/theme-provider"
import { Toaster } from "sonner"
import { SignalProvider } from "@/lib/contexts/SignalContext"

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
          <SignalProvider>
            {children}
            <Toaster position="top-right" richColors />
          </SignalProvider>
        </ThemeProvider>
      </body>
    </html>
  )
}
