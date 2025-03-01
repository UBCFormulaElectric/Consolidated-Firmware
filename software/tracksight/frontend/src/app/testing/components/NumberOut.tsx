import React, { useState, useEffect } from "react";
import io from "socket.io-client";

interface DataPoint {
   time: number | string;
   [signalName: string]: number | string | undefined;
}

interface SignalMeta {
  name: string;
  unit: string;
  cycle_time_ms?: number;
}

interface NumberOutProps {
  currentTime: number;
  numericalSignals: string[];
}

const NumberOut: React.FC<NumberOutProps> = ({
  currentTime,
  numericalSignals,
}) => {
  const [data, setData] = useState<DataPoint[]>([]);
  const [activeSignals, setActiveSignals] = useState<string[]>([]);
  const [selectedSignal, setSelectedSignal] = useState<string>("");
  const [availableSignals, setAvailableSignals] = useState<SignalMeta[]>([]);
  const [socket, setSocket] = useState<any>(null);

  // Fetch signals on load
  useEffect(() => {
    fetch("http://localhost:5000/api/signal")
      .then((response) => response.json())
      .then((signals) => {
        const signalsMeta: SignalMeta[] = signals.map((signal: any) => ({
          name: signal.name,
          unit: signal.unit,
          cycletime_ms: signal.cycle_time_ms,
        }));
        setAvailableSignals(signalsMeta);
        console.log("Fetched available signals:", signalsMeta);
      })
      .catch((error) => console.error("Error fetching signals:", error));
  }, []);

  // Establish socket connection
  useEffect(() => {
    const newSocket = io("http://localhost:5000");
    setSocket(newSocket);

    newSocket.on("connect", () => {
      console.log("Connected to WebSocket server");
    });

    // Clean up on unmount
    return () => {
      newSocket.disconnect();
    };
  }, []);

  // Subscribe to a signal when it's selected from the dropdown.
  useEffect(() => {
    if (socket && selectedSignal && !activeSignals.includes(selectedSignal)) {
      socket.emit("sub", selectedSignal);
      console.log("Subscribing to signal:", selectedSignal);
      setActiveSignals((prevSignals) => [...prevSignals, selectedSignal]);
      setSelectedSignal("");
    }
  }, [socket, selectedSignal, activeSignals]);

  // Handle live data and only log data for subscribed signals.
  useEffect(() => {
    if (!socket) return;


    const dataHandler = (incomingData: any) => {
        // incomingData is expected to be an aggregated object formatted by the broadcaster:
        // { time: "timestamp", SIGNAL_NAME1: value1, SIGNAL_NAME2: value2, ... }
        console.log("Full received data:", incomingData);
        const filteredData: { [key: string]: number } = {};
        activeSignals.forEach((signal) => {
          if (incomingData.hasOwnProperty(signal)) {
            filteredData[signal] = incomingData[signal];
          }
        });
  
        if (Object.keys(filteredData).length > 0) {
          console.log("Received data for subscribed signals:", filteredData); // CHANGED
          // Save the entire aggregated data point
          setData((prevData) => [...prevData, incomingData]); // CHANGED
        }
      };

    socket.on("data", dataHandler);
    

    // Cleanup when activeSignals change or component unmounts.
    return () => {
      socket.off("data", dataHandler);
    };
  }, [socket, activeSignals]);

  // Unsubscribe when a signal is removed
  const removeSignal = (signalName: string) => {
    if (socket) {
      console.log("Unsubscribing to signal");
      socket.emit("unsub", signalName);
    }
    setActiveSignals((prevSignals) =>
      prevSignals.filter((sig) => sig !== signalName)
    );
  };

  useEffect(() => {
    if (!socket) return;
  
    const subAckHandler = (ack: { signal: string; status: string }) => {  // CHANGED
      console.log(`Subscription acknowledgment received: Signal ${ack.signal} - Status: ${ack.status}`);  // CHANGED
    };
  
    socket.on("sub_ack", subAckHandler);  // CHANGED
  
    return () => {
      socket.off("sub_ack", subAckHandler);  // CHANGED
    };
  }, [socket]);
  
  return (
    <div className="w-full h-64">
      <div className="ml-24 mt-4 text-xs flex-column gap-4">
        {activeSignals.map((signalName) => {
          // Retrieve the latest value for this signal from the data array
          const latestValue =
            data.length > 0 ? data[data.length - 1][signalName] : undefined;
          // Find metadata to get the unit
          const meta = availableSignals.find((s) => s.name === signalName);
          return (
            <div key={signalName} className="flex items-center mb-1">
              <span className="text-gray-500">{signalName}: </span>
              <span className="ml-2">
                {latestValue !== undefined
                  ? `${latestValue} ${meta ? meta.unit : ""}`
                  : "N/A (no data)"}
              </span>
              <button
                className="ml-2 text-red-500"
                onClick={() => removeSignal(signalName)}
              >
                Remove
              </button>
            </div>
          );
        })}
      </div>

      <div className="ml-24 mt-4">
        <select
          value={selectedSignal}
          onChange={(e) => setSelectedSignal(e.target.value)}
          className="mr-2 mb-2 p-2 bg-white border border-gray-300"
        >
          <option value="" disabled>
            Select a signal
          </option>
          {availableSignals
            .filter((signal) => !activeSignals.includes(signal.name))
            .map((signal) => (
              <option key={signal.name} value={signal.name}>
                {signal.name} ({signal.unit})
              </option>
            ))}
        </select>
      </div>
    </div>
  );
};

export default NumberOut;