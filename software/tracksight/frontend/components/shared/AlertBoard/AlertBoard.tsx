import { useSignals } from "@/lib/contexts/SignalContext";
import { AlertCircle, AlertTriangle, Info } from "lucide-react";
import { useState } from "react";

interface Alert {
  id: string;
  type: "Fault" | "Warning" | "Info";
  name: string;
  lastUpdated: string;
}

type Severity = "HIGH" | "MEDIUM" | "LOW";
type AlertType = "Fault" | "Warning" | "Info";

export default function AlertBoard() {
  const { data } = useSignals();
  console.log(data);
  // Separate state for each alert type
  const [faults, setFaults] = useState<Alert[]>([
    {
      id: "1",
      type: "Fault",
      name: "Battery Overheating",
      lastUpdated: "2023-05-15 14:30",
    },
    {
      id: "2",
      type: "Fault",
      name: "Motor Failure",
      lastUpdated: "2023-05-15 10:15",
    },
  ]);

  const [warnings, setWarnings] = useState<Alert[]>([
    {
      id: "3",
      type: "Warning",
      name: "Low Tire Pressure",
      lastUpdated: "2023-05-15 14:25",
    },
    {
      id: "5",
      type: "Warning",
      name: "Low Battery",
      lastUpdated: "2023-05-15 15:10",
    },
  ]);

  const [infoAlerts, setInfoAlerts] = useState<Alert[]>([
    {
      id: "4",
      type: "Info",
      name: "Software Update Available",
      lastUpdated: "2023-05-14 09:15",
    },
  ]);

  const getTypeIcon = (type: AlertType) => {
    switch (type) {
      case "Fault":
        return <AlertTriangle className="h-4 w-4" />;
      case "Warning":
        return <AlertCircle className="h-4 w-4" />;
      case "Info":
        return <Info className="h-4 w-4" />;
      default:
        return <AlertTriangle className="h-4 w-4" />;
    }
  };

  const getTypeConfig = (type: AlertType) => {
    switch (type) {
      case "Fault":
        return {
          borderColor: "border-red-300",
          headerColor: "bg-red-50 border-b-red-300",
          iconColor: "text-red-500",
          textColor: "text-red-700",
          description: "Critical issues requiring immediate attention",
        };
      case "Warning":
        return {
          borderColor: "border-yellow-300",
          headerColor: "bg-yellow-50 border-b-yellow-300",
          iconColor: "text-yellow-500",
          textColor: "text-yellow-700",
          description: "Important notifications to monitor",
        };
      case "Info":
        return {
          borderColor: "border-blue-300",
          headerColor: "bg-blue-50 border-b-blue-300",
          iconColor: "text-blue-500",
          textColor: "text-blue-700",
          description: "Informational messages",
        };
      default:
        return {
          borderColor: "border-gray-300",
          headerColor: "bg-gray-50 border-b-gray-300",
          iconColor: "text-gray-500",
          textColor: "text-gray-700",
          description: "",
        };
    }
  };

  const renderAlertList = (type: AlertType) => {
    const alerts = {
      Fault: faults,
      Warning: warnings,
      Info: infoAlerts,
    }[type];

    const typeConfig = getTypeConfig(type);

    return (
      <div className={`rounded-lg border ${typeConfig.borderColor} shadow-sm`}>
        {/* Header */}
        <div
          className={`px-4 py-3 ${typeConfig.headerColor} border-b flex items-center justify-between`}
        >
          <div className="flex items-center gap-3">
            <div className={`p-2 rounded-full ${typeConfig.iconColor}`}>
              {getTypeIcon(type)}
            </div>
            <div>
              <h2 className={`font-semibold ${typeConfig.textColor}`}>
                {type} Alerts
              </h2>
              <p className="text-xs text-gray-500">{typeConfig.description}</p>
            </div>
          </div>
          <span className="text-lg font-bold">{alerts.length}</span>
        </div>

        {/* Alert Items */}
        <div className="divide-y divide-gray-200">
          {alerts.length === 0 ? (
            <div className="px-4 py-6 text-center text-gray-500 text-sm">
              No {type.toLowerCase()} alerts
            </div>
          ) : (
            alerts.map((alert) => (
              <div
                key={alert.id}
                className="px-4 py-3 hover:bg-gray-50 transition-colors"
              >
                <div className="flex justify-between items-start">
                  <div>
                    <h3 className="font-medium text-gray-900">{alert.name}</h3>
                  </div>
                  <span className="text-xs text-gray-500 whitespace-nowrap">
                    Last seen: {alert.lastUpdated}
                  </span>
                </div>
              </div>
            ))
          )}
        </div>
      </div>
    );
  };

  return (
    <div className="mx-auto p-4 sm:p-6">
      <div className="grid grid-cols-1 lg:grid-cols-3 gap-4">
        {renderAlertList("Fault")}
        {renderAlertList("Warning")}
        {renderAlertList("Info")}
      </div>
    </div>
  );
}
