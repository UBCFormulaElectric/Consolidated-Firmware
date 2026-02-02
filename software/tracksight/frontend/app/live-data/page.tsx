"use client";

import LiveDataDashboard from "@/components/LiveDataDashboard";
import { DashboardLayoutProvider } from "@/lib/contexts/DashboardLayout";

import { LiveSignalStoreProvider } from "@/lib/contexts/signalStores/LiveSignalStoreContext";
import { MockSignalStoreProvider } from "@/lib/contexts/signalStores/MockSignalStoreContext";

const USE_MOCK_DATA = true;

export default function LiveDataPage() {
  const DataSourceProvider = USE_MOCK_DATA ? MockSignalStoreProvider : LiveSignalStoreProvider;

  return (
    <div className="w-screen pb-20">
      <DashboardLayoutProvider>
        <DataSourceProvider>
          <LiveDataDashboard />
        </DataSourceProvider>
      </DashboardLayoutProvider>
    </div>
  );
}

