"use client";

import LiveDataDashboard from "@/components/LiveDataDashboard";
import { DashboardLayoutProvider } from "@/lib/contexts/DashboardLayout";
import { useEditMode } from "@/lib/contexts/EditModeContext";

export default function LiveDataPage() {
  const { isEditMode } = useEditMode();

  return (
    <div className="overflow-y-scroll p-12 w-screen">
      <DashboardLayoutProvider>
        {isEditMode
          ? <></>
          : <LiveDataDashboard />
        }
      </DashboardLayoutProvider>
    </div>
  );
}
