"use client";

// import AlertBoard from "@/components/shared/AlertBoard/AlertBoard";
// import ConnectionStatus from "@/components/pages/live-data/ConnectionStatus";
import DynamicRowManager from "@/components/shared/DynamicRowManager";
import { DisplayControlProvider } from "@/components/shared/PausePlayControl";
import SyncedGraphContainer from "@/components/shared/SyncedGraphContainer";
import { SignalProvider } from "@/hooks/SignalContext";

import LiveDataDashboard from "@/components/LiveDataDashboard";
import { DashboardLayoutProvider } from "@/lib/contexts/DashboardLayout";


export default function LiveDataPage() {
  return (
    // <DisplayControlProvider>
    //   <SignalProvider>
    //     <div className="pt-24 min-h-screen">
    //       <div className="sticky w-full left-0">
    //         <h2 className="text-xl font-bold mb-2">Connection Status</h2>
    //         {/* <ConnectionStatus /> */}

    //         {/* TODO rework alerts */}
    //         {/* <h2 className="text-xl font-bold mb-2">Alerts</h2>
    //         <AlertBoard /> */}

    //         <h2 className="text-xl font-bold mb-2">
    //           Add New Signal Subscriptions
    //         </h2>
    //         <SyncedGraphContainer className="mt-4">
    //           <DynamicRowManager />
    //         </SyncedGraphContainer>
    //       </div>
    //     </div>
    //   </SignalProvider>
    // </DisplayControlProvider>

    <div>
      <DashboardLayoutProvider>
        <LiveDataDashboard />
      </DashboardLayoutProvider>
    </div>
  );
}
