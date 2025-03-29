"use client";

import { useState } from "react";
import Visualize from "./visualize";

const FLASK_URL = "http://localhost:5000";

export default function VisualizePage() {
  const [graphs, setGraphs] = useState<number[]>([]);
  const [liveGraphs, setLiveGraphs] = useState<number[]>([]);

  //add a new graph
  const addGraph = (live: boolean) => {
    const newGraphId = Date.now();
    if (live) {
      setLiveGraphs((prevGraphs) => [...prevGraphs, newGraphId]);
    } else {
      setGraphs((prevGraphs) => [...prevGraphs, newGraphId]);
    }
  };

  //delete a graph
  const deleteGraph = (graphId: number, live: boolean) => {
    if (live) {
      setLiveGraphs((prevGraphs) => prevGraphs.filter((id) => id !== graphId));
    } else {
      setGraphs((prevGraphs) => prevGraphs.filter((id) => id !== graphId));
    }
  };

  return (
    <Visualize
      addGraph={addGraph}
      graphs={graphs}
      liveGraphs={liveGraphs}
      deleteGraph={deleteGraph}
      url={FLASK_URL}
    />
  );
}
