import LiveDataDashboard from "@/components/LiveDataDashboard";
import { DashboardLayoutProvider } from "@/lib/contexts/DashboardLayout";

export default function LiveDataPage() {
  return (
    <div className="w-screen overflow-y-visible">
      <DashboardLayoutProvider>
        <LiveDataDashboard />
      </DashboardLayoutProvider>
    </div>
  );
}
