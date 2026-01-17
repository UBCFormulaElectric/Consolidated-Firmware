"use client";

import LiveDataDashboard from "@/components/LiveDataDashboard";
import { DashboardLayoutProvider } from "@/lib/contexts/DashboardLayout";
import { LiveSignalDataStoreProvider } from "@/lib/contexts/LiveSignalDataStore";

export default function LiveDataPage() {
  return (
    <div className="w-screen pb-20">
      <DashboardLayoutProvider>
        <LiveSignalDataStoreProvider>
          <LiveDataDashboard />
        </LiveSignalDataStoreProvider>
      </DashboardLayoutProvider>
    </div>
  );
}

