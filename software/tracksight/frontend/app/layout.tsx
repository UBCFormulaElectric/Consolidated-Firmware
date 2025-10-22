import "@/app/globals.css";

import type { Metadata } from "next";
import { Inter } from "next/font/google";
import type React from "react";

import { fetchSignalMetadata } from "@/lib/api/signals";
import { EditModeProvider } from "@/lib/contexts/EditModeContext";
import { SignalsMetadataProvider } from "@/lib/contexts/SignalsMetadataContext";

const inter = Inter({ subsets: ["latin"] });

import Sidebar from "@/components/Navbar";
import { API_BASE_URL } from "@/lib/constants";

export const metadata: Metadata = {
  title: "Tracksight Dashboard",
  description: "Monitoring and visualization dashboard for system data",
};

export default async function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  let initialSignalData;

  try {
    initialSignalData = await fetchSignalMetadata(API_BASE_URL);
  } catch (error) {
    console.error("Failed to preload signal metadata:", error);

    // NOTE(evan): Continue rendering with undefined signal data, client can
    //             take over and attempt to get the data.
    initialSignalData = undefined;
  }

  return (
    <html lang="en" suppressHydrationWarning>
      <body className={`${inter.className} bg-primary-200 overflow-y-scroll pb-20 m-0`}>
        <SignalsMetadataProvider apiBaseUrl={API_BASE_URL} initialData={initialSignalData}>
          <EditModeProvider>
            <Sidebar />
            <main className="mt-24 overflow-y-visible py-12">{children}</main>
          </EditModeProvider>
        </SignalsMetadataProvider>
      </body>
    </html>
  );
}
