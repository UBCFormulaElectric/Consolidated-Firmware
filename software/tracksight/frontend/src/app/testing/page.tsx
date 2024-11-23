"use client";

import React, { useState, useEffect } from "react";
import LiveFault from "./components/LiveFault";
import Timer from "./components/Timer";
import Live from "./components/Live";
import EnumerationGraph from "./components/Enumeration";

const TestingPage = () => {
  const [currentTime, setCurrentTime] = useState(Math.floor(Date.now() / 1000));
  const [currentState, setCurrentState] = useState("VC_INIT_STATE");

  const enumStates = [
    "VC_INIT_STATE",
    "VC_PCM_STATE",
    "VC_HV_STATE",
    "VC_DRIVE_STATE",
    "VC_VD_STATE",
  ];

  useEffect(() => {
    const timeInterval = setInterval(() => {
      setCurrentTime(Math.floor(Date.now() / 1000));
    }, 1000);

    const stateInterval = setInterval(() => {
      setCurrentState((prevState) => {
        const nextIndex = Math.floor(Math.random() * enumStates.length);
        return enumStates[nextIndex];
      });
    }, 10000);

    return () => {
      clearInterval(timeInterval);
      clearInterval(stateInterval);
    };
  }, []);

  return (
    <div>
      <Live />
      {/* <MouseTest /> */}
      <Timer />
      <LiveFault />
      <EnumerationGraph
        signalName="VC_STATE"
        currentState={currentState}
        currentTime={currentTime}
        enumStates={enumStates}
      />
    </div>
  );
};

export default TestingPage;

// TODO: Consider using ScrollArea https://ui.shadcn.com/docs/components/scroll-area
