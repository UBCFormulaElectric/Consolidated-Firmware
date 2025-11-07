import "@/app/globals.css";

import type { Metadata } from "next";
import { Inter } from "next/font/google";
import type { ReactNode } from "react";

const inter = Inter({ subsets: ["latin"] });

import Navbar from "@/components/Navbar";
import QueryProvider from "@/lib/contexts/QueryProvider";

export const metadata: Metadata = {
  title: "Tracksight Dashboard",
  description: "Monitoring and visualization dashboard for system data",
};

export default async function RootLayout({
  children,
}: Readonly<{
  children: ReactNode;
}>) {
  return (
    <html lang="en" suppressHydrationWarning>
      <body className={`${inter.className} bg-primary-200 overflow-y-scroll pb-20`}>
        <QueryProvider>
          <Navbar />
          <main className="mt-24 py-12">{children}</main>
        </QueryProvider>
      </body>
    </html>
  );
}
