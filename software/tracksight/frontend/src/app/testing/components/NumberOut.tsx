import React, { useState, useEffect } from "react";
import io from "socket.io-client";

interface DataPoint {
  name: string;
  value: unknown;
  timestamp: string;
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
          cycle_time_ms: signal.cycle_time_ms, // still unsure what this cycle_time_ms is for
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

  // Handle live data and log detailed fields from incoming data.
  useEffect(() => {
    if (!socket) return;

    const dataHandler = (incomingData: any) => {
      // Only store the data if its name is in activeSignals
      if (activeSignals.includes(incomingData.name)) {
        console.log(
          `Received data for subscribed signal ${incomingData.name}:`,
          incomingData
        );
        setData((prevData) => [...prevData, incomingData]);
      }
    };

    socket.on("data", dataHandler);

    return () => {
      socket.off("data", dataHandler);
    };
  }, [socket, activeSignals]);

  // Unsubscribe when a signal is removed
  const removeSignal = (signalName: string) => {
    if (socket) {
      console.log("Unsubscribing from signal", signalName);
      socket.emit("unsub", signalName);
    }
    setActiveSignals((prevSignals) =>
      prevSignals.filter((sig) => sig !== signalName)
    );
  };

  useEffect(() => {
    if (!socket) return;

    const subAckHandler = (ack: { signal: string; status: string }) => {
      console.log(
        `Subscription acknowledgment received: Signal ${ack.signal} - Status: ${ack.status}`
      );
    };

    socket.on("sub_ack", subAckHandler);

    return () => {
      socket.off("sub_ack", subAckHandler);
    };
  }, [socket]);

  return (
    <div className="w-full h-64">
      <div className="ml-24 mt-4 text-xs flex flex-col gap-4">
        {activeSignals.map((signalName) => {
          // Find the latest data point for the signal
          const latestData = data
            .filter((d) => d.name === signalName)
            .slice(-1)[0];
          // Find metadata to get the unit
          const meta = availableSignals.find((s) => s.name === signalName);
          return (
            <div key={signalName} className="flex items-center mb-1">
              <span className="text-gray-500">{signalName}: </span>
              <span className="ml-2">
                {latestData !== undefined
                  ? `${latestData.value} ${meta ? meta.unit : ""} at (${latestData.timestamp})`
                  : "N/A (no data)"}
              </span>
              <button
                className="ml-2 text-red-500"
                onClick={() => removeSignal(signalName)}>
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
          className="mr-2 mb-2 p-2 bg-white border border-gray-300">
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