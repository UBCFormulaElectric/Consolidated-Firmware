"use client";

import DataDashboard from "@/components/DataDashboard";
import { DashboardLayoutProvider } from "@/lib/contexts/DashboardLayout";
import { ScrollProvider } from "@/lib/contexts/ScrollContext";

import { LiveSignalStoreProvider } from "@/lib/contexts/signalStores/LiveSignalStoreContext";
import { MockSignalStoreProvider } from "@/lib/contexts/signalStores/MockSignalStoreContext";

const USE_MOCK_DATA = true;

export default function LiveDataPage() {
  const DataSourceProvider = USE_MOCK_DATA ? MockSignalStoreProvider : LiveSignalStoreProvider;

  return (
    <div className="w-screen pb-20">
      <DashboardLayoutProvider>
        <ScrollProvider initialTimestamp={new Date().getTime()}>
          <DataSourceProvider>
            <DataDashboard />
          </DataSourceProvider>
        </ScrollProvider>
      </DashboardLayoutProvider>
    </div>
  );
}
