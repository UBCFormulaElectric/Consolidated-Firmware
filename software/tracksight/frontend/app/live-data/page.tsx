"use client";

import AlertBoard from "@/components/shared/AlertBoard/AlertBoard";
import DynamicRowManager from "@/components/shared/DynamicRowManager";
import ConnectionStatus from "@/components/pages/live-data/ConnectionStatus";

export default function LiveDataPage() {
  return (
    <div className="overflow-x-scroll pt-24 min-h-screen space-y-6">
      <h2 className="text-xl font-bold mb-2">Connection Status</h2>
      <ConnectionStatus />

      {/* TODO rework alerts */}
      {/* <h2 className="text-xl font-bold mb-2">Alerts</h2>
      <AlertBoard /> */}

      <h2 className="text-xl font-bold mb-2">Add New Signal Subscriptions</h2>
      <DynamicRowManager />


      <div className="bg-orange-500 h-24 right-0 absolute" style={{ width: "500px" }} />
    </div>
  );
}
