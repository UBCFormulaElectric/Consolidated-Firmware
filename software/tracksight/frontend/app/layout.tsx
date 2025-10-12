import '@/app/globals.css'

import type React from "react"
import type { Metadata } from "next"
import { Inter } from "next/font/google";

import { EditModeProvider } from '@/lib/contexts/EditModeContext';
import { SignalsMetadataProvider } from '@/lib/contexts/SignalsContext';
import { fetchSignalMetadata } from '@/lib/api/signals';

const inter = Inter({ subsets: ["latin"] });

import Sidebar from "@/components/Navbar"

export const metadata: Metadata = {
  title: "Tracksight Dashboard",
  description: "Monitoring and visualization dashboard for system data",
}

const API_BASE_URL = process.env.NEXT_PUBLIC_API_BASE_URL || "http://127.0.0.1:5001/";

export default async function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode
}>) {
  let initialSignalData;

  try {
    initialSignalData = await fetchSignalMetadata(API_BASE_URL);
  } catch (error) {
    console.error('Failed to preload signal metadata:', error);
    
    // NOTE(evan): Continue rendering with undefined signal data, client can
    //             take over and attempt to get the data.
    initialSignalData = undefined;
  }

  return (
    <html lang="en" suppressHydrationWarning>
      <body className={`${inter.className} bg-primary-200`}>
        <SignalsMetadataProvider 
          apiBaseUrl={API_BASE_URL}
          initialData={initialSignalData}
        >
          <EditModeProvider>
            <Sidebar />
            <main className="mt-24">{children}</main>
          </EditModeProvider>
        </SignalsMetadataProvider>
      </body>
    </html>
  )
}
