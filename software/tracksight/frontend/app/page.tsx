import LiveDataDashboard from "@/components/LiveDataDashboard";
import { DashboardLayoutProvider } from "@/lib/contexts/DashboardLayout";

export default function LiveDataPage() {
  return (
    <div className="p-12 w-screen overflow-y-visible">
      <DashboardLayoutProvider>
        <LiveDataDashboard />
      </DashboardLayoutProvider>
    </div>
  );
}
