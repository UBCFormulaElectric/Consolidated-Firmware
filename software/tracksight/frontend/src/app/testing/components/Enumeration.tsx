import React, { useState, useEffect, useRef } from "react";

interface EnumerationGraphProps {
  signalName: string;
  currentState: string;
  currentTime: number;
  enumStates: string[];
}

interface StateHistoryItem {
  state: string;
  startTime: number;
}

interface StateBar {
  state: string;
  startTime: number;
  left: number;
  width: number;
}

const EnumerationGraph: React.FC<EnumerationGraphProps> = ({
  signalName,
  currentState,
  currentTime,
  enumStates,
}) => {
  const containerRef = useRef<HTMLDivElement>(null);
  const [stateHistory, setStateHistory] = useState<StateHistoryItem[]>([]);

  // Define the total time window displayed on the graph (e.g., last 60 seconds)
  const timeWindow = 11 * 1000; // milliseconds

  useEffect(() => {
    // Update state history when currentState changes
    setStateHistory((prevHistory) => {
      const lastState = prevHistory[prevHistory.length - 1];
      if (!lastState || lastState.state !== currentState) {
        // If state changed, add new state with current time
        return [
          ...prevHistory,
          { state: currentState, startTime: currentTime },
        ];
      } else {
        // State didn't change, return previous history
        return prevHistory;
      }
    });
  }, [currentState]);

  useEffect(() => {
    // Remove states that end before the current time window
    setStateHistory((prevHistory) =>
      prevHistory.filter((state, index) => {
        const nextState = prevHistory[index + 1];
        const endTime = nextState ? nextState.startTime : currentTime;
        return endTime > currentTime - timeWindow;
      })
    );
  }, [currentTime]);

  // TODO: actual signals will have variable width based on time window,
  // will return data points at unspecified intervals and will need to be interpolated

  const containerWidth = containerRef.current?.offsetWidth || window.innerWidth;
  const pixelsPerMs = containerWidth / timeWindow;

  const stateBars: StateBar[] = stateHistory.map((state, index) => {
    const nextState = stateHistory[index + 1];

    const stateStartTime = state.startTime;
    const stateEndTime = nextState ? nextState.startTime : currentTime;

    const barStartTime = Math.max(stateStartTime, currentTime - timeWindow);
    const barEndTime = Math.min(stateEndTime, currentTime);

    const startOffset =
      (barStartTime - (currentTime - timeWindow)) * pixelsPerMs;
    const duration = barEndTime - barStartTime;
    const width = duration * pixelsPerMs;

    return {
      ...state,
      left: startOffset,
      width,
    };
  });

  // Colors for the states, mapping to indices
  const stateColors: string[] = [
    "#FF3B2F",
    "#FFCC02",
    "#FF9500",
    "#35C759",
    "#007AFF",
  ];

  const getStateColor = (state: string): string => {
    const index = enumStates.indexOf(state);
    return stateColors[index % stateColors.length]; // Wrap around if more states
  };

  return (
    <div className="w-full relative">
      {/* Signal Name and Legend */}
      <div className="mx-4 flex gap-5 items-center">
        <div className="font-bold my-2 text-white bg-blue-500 rounded-full inline-block p-1 text-sm">
          {signalName}
        </div>
        {/* Legend */}
        <div className="mt-2 text-xs flex gap-4">
          {enumStates.map((state, index) => (
            <div key={index} className="flex items-center mb-1">
              <span
                className="w-3 h-3 inline-block mr-2"
                style={{ backgroundColor: getStateColor(state) }}
              ></span>
              <span className="text-gray-500">{state}</span>
            </div>
          ))}
        </div>
      </div>
      {/* Graph */}
      <div
        className="relative w-full h-6 bg-gray-100 overflow-hidden"
        ref={containerRef}
      >
        {stateBars.map(
          (bar, index) =>
            bar.width > 0 && (
              <div
                key={index}
                className="absolute top-0 bottom-0"
                style={{
                  left: `${bar.left}px`,
                  width: `${bar.width}px`,
                  backgroundColor: getStateColor(bar.state),
                }}
                title={`${bar.state} (${new Date(
                  bar.startTime
                ).toLocaleTimeString()})`}
              ></div>
            )
        )}
      </div>
    </div>
  );
};

export default EnumerationGraph;
