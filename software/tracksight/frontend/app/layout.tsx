import "@/app/globals.css";

import type { Metadata } from "next";
import { Inter } from "next/font/google";
import type React from "react";

import { SignalsMetadataProvider } from "@/lib/contexts/SignalsMetadataContext";

const inter = Inter({ subsets: ["latin"] });

import Navbar from "@/components/Navbar";

export const metadata: Metadata = {
  title: "Tracksight Dashboard",
  description: "Monitoring and visualization dashboard for system data",
};

export default async function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  return (
    <html lang="en" suppressHydrationWarning>
      <body className={`${inter.className} bg-primary-200 overflow-y-scroll pb-20`}>
        <SignalsMetadataProvider>
          <Navbar />
          <main className="mt-24 overflow-y-visible py-12">{children}</main>
        </SignalsMetadataProvider>
      </body>
    </html>
  );
}
