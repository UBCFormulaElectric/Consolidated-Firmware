import AlertBar from "@/components/AlertBar";
import LiveDataDashboard from "@/components/LiveDataDashboard";
import { DashboardLayoutProvider } from "@/lib/contexts/DashboardLayout";


export default function LiveDataPage() {
  return (
    <div className="w-screen pb-20">
      <DashboardLayoutProvider>
        <LiveDataDashboard />
        <AlertBar />
      </DashboardLayoutProvider>
    </div>
  );
}

