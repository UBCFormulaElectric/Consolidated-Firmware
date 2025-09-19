"use client";

import Sidebar from "@/components/shared/Sidebar";
import { usePathname } from "next/navigation";
import { useEffect, useState } from "react";

interface LayoutWrapperProps {
  children: React.ReactNode;
}

export default function LayoutWrapper({ children }: LayoutWrapperProps) {
  const [activePage, setActivePage] = useState<string>("");
  const pathname = usePathname();

  useEffect(() => {
    // Determine active page based on current pathname
    if (pathname === "/live-data") {
      setActivePage("live-data");
    } else if (pathname === "/visualize") {
      setActivePage("visualize");
    } else if (pathname === "/dashboard") {
      setActivePage("dashboard");
    } else {
      setActivePage("live-data"); // Default to live-data
    }
  }, [pathname]);

  return (
    <div className="flex flex-col h-screen bg-gray-100 dark:bg-gray-900">
      <Sidebar activePage={activePage} setActivePage={setActivePage} />
      <main className="flex-1 overflow-auto pb-36">{children}</main>
    </div>
  );
}
