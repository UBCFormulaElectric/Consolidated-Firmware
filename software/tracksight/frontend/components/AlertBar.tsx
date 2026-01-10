"use client";

import { useState } from 'react';
import { ChevronUp, Clock4, TriangleAlert, X } from 'lucide-react';

type AlertProps = {
  signalName: string;
  txNode: string;
  timestamp: string;
  onDismiss?: () => void;
}

function Alert(props: AlertProps) {
  const {
    signalName,
    txNode,
    timestamp,
    onDismiss
  } = props;

  const hourlyTimestamp = new Date(timestamp).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit', second: '2-digit', hour12: false });

  return (
    <div className="border-l-2 border-red-400 bg-red-100/50 p-4 w-full flex justify-between">
      <div className="flex gap-4">
        <TriangleAlert className="text-red-400 size-6" />
        <div className="flex flex-col">
          <div className="flex flex-row h-6 items-center">
            <p className="font-semibold">{signalName}</p>
          </div>
          <div className="mt-2 flex gap-2 text-gray-600">
            <p className="text-sm">{txNode}</p>
            <div className="flex items-center gap-1 text-sm">
              <Clock4 className="size-4" />
              <span>{hourlyTimestamp}</span>
            </div>
          </div>
        </div>
      </div>
      <div className="flex items-center">
        <button
          className="hover:cursor-pointer p-2 rounded-md group hover:bg-primary-200 transition-colors"
          onClick={onDismiss}
        >
          <X className="size-5 text-gray-600 group-hover:text-red-400" />
        </button>
      </div>
    </div>
  );
}

type AlertSelectionBarProps = {
  onAddAlert: () => void;
  onRemoveAlert: () => void;

  subscribedAlerts?: string[];
}

function AlertSelectionBar(props: AlertSelectionBarProps) {
  const { onAddAlert, onRemoveAlert, subscribedAlerts } = props;

  const [isExpanded, setIsExpanded] = useState(false);

  return (
    <div className="flex flex-col p-6 w-full border-b-2 border-primary-300">
      <div className="flex items-center gap-2">
        <span>
          Subscribed Alerts ({subscribedAlerts ? subscribedAlerts.length : 0})
        </span>

        <ChevronUp
          className={`${isExpanded ? "rotate-180" : "rotate-90"} hover:cursor-pointer size-6 transition-all`}
          onClick={() => setIsExpanded(!isExpanded)}
        />
      </div>

      <div className={`${isExpanded ? "max-h-52" : "max-h-0"} transition-all overflow-y-scroll duration-300 ease-in-out`}>
        <div className="mt-4">
          {
            subscribedAlerts && subscribedAlerts.length > 0 ? (
              <div className="flex flex-wrap gap-4">
                {subscribedAlerts.map((alert, index) => (
                  <div
                    key={index}
                    className="flex items-center gap-2 border border-primary-300 rounded-full px-4 py-2 bg-primary-200"
                  >
                    <span>{alert}</span>
                    <button
                      className="hover:bg-red-100 hover:cursor-pointer p-1 rounded-full transition-colors"
                      onClick={onRemoveAlert}
                    >
                      <X className="size-4 text-gray-600 hover:text-red-400" />
                    </button>
                  </div>
                ))}
              </div>
            ) : (
              <p className="text-gray-600">
                No subscribed alerts.
              </p>
            )
          }
        </div>
      </div >
    </div >
  );
}


function AlertBar() {
  const [isExpanded, setIsExpanded] = useState(false);

  return (
    <div className="fixed bottom-0 border-primary-300 border-t-2 bg-primary-100 left-0 z-50 flex w-screen flex-col">
      <div className="p-6 text-xl flex justify-between items-center" >
        <div>
          <span>
            Alerts and Errors
          </span>
        </div>

        <div>
          <button
            className="hover:bg-primary-200 hover:cursor-pointer p-2 rounded-md transition-colors"
            onClick={() => setIsExpanded(!isExpanded)}
          >
            <ChevronUp className={`${isExpanded && "rotate-180"} transition-all`} />
          </button>
        </div>
      </div >

      <div className={`${isExpanded ? "max-h-56" : "max-h-0"} transition-all overflow-y-scroll scrollbar-hidden duration-300 ease-in-out border-t-2 border-primary-300`}>
        <AlertSelectionBar
          onAddAlert={() => { }}
          onRemoveAlert={() => { }}
        />

        <div className="p-6 space-y-4">
          <Alert signalName="Battery Alert 1" txNode="BMS" timestamp="2024-06-15 14:32:00" />
          <Alert signalName="Battery Alert 2" txNode="BMS" timestamp="2024-06-15 14:35:00" />
          <Alert signalName="Some other ALert" txNode="VMC" timestamp="2024-06-15 14:40:00" />
        </div>
      </div>
    </div >
  );
};

export default AlertBar;
