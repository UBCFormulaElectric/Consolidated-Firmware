import LiveDataDashboard from "@/components/LiveDataDashboard";
import { DashboardLayoutProvider } from "@/lib/contexts/DashboardLayout";

export default function LiveDataPage() {
  return (
    <div className="overflow-y-scroll p-12 w-screen">
      <DashboardLayoutProvider>
        <LiveDataDashboard />
      </DashboardLayoutProvider>
    </div>
  );
}
