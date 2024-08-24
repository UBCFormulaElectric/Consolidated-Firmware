import React, { useState, ReactElement } from "react";
import { Divider, Button, Switch, Space, Modal, Input, message } from "antd";
import Graph from "./influxcomponents/graph";
import LiveGraph from "./livecomponents/live_graph";
import { PlotRelayoutEvent } from "plotly.js";
// import { saveDashboardData } from '../components/dashboardService'

export default function Visualize(props: {
  addGraph: (live: boolean) => void;
  graphs: number[];
  liveGraphs: number[];
  url: string;
  deleteGraph: (graphId: number, live: boolean) => void;
}) {
  const [sync, setSync] = useState<boolean>(false);
  const [zoomData, setZoomData] = useState<PlotRelayoutEvent>({});
  // const [dbName, setDbName] = useState<string>("test");
  const [modalOpen, setModalOpen] = useState<boolean>(false);

  // const handleInputChange = (event: React.ChangeEvent<HTMLInputElement>) => {
  //     setDbName(event.target.value);
  // }
  const changeSync = (checked: boolean) => {
    setSync(checked);
  };

  // const saveDashboard = async () => {
  //     //error handling for cases where these goblins try to save dashboard without a name or without any data
  //     if (!dbName.trim()) {
  //         message.error('name your dashboard before saving brother');
  //         return;
  //     }
  //     const hasData = Object.keys(graphSignals).some(graphId => graphSignals[graphId].length > 0);
  //     if (!hasData) {
  //         message.warning('stop trying to save a dashboard without any graphs brother');
  //         return;
  //     }
  //     const data: { dbname: string; graphs: Record<string, string[]> } = {
  //         dbname: dbName,
  //         graphs: {}
  //     };
  //     for (let graphId in graphSignals) {
  //         data.graphs[graphId] = graphSignals[graphId];
  //     }
  //     const path = `dashboards/${dbName}`;
  //     const success = await saveDashboardData(path, data);
  //     if (success) {
  //         message.success('Dashboard saved successfully!');
  //         setModalOpen(false);
  //     } else {
  //         message.error('Error saving the dashboard.');
  //     }
  //     setModalOpen(false);
  // };
  return (
    <div className="layout">
      <Space direction="vertical" size="large">
        <div>
          <h1>Visualize</h1>
          <p>Select a graph to provide live data or data from InfluxDB.</p>
        </div>
        <Space size="middle">
          <Space direction="vertical" size="small">
            <p>Sync Zoom</p>
            <Switch onChange={changeSync} checked={sync} />
          </Space>
          <Button onClick={() => props.addGraph(false)}>
            Add Influx Graph
          </Button>
          <Button onClick={() => props.addGraph(true)}>Add Live Graph</Button>
          <Button onClick={() => setModalOpen(true)}>
            Save Current Loadout
          </Button>
          <Modal
            open={modalOpen}
            closeIcon={false}
            title="Save Dashboard"
            cancelButtonProps={{ onClick: () => setModalOpen(false) }}
          >
            {/* <Input placeholder='Name of dashboard...' onChange={handleInputChange}></Input> */}
          </Modal>
          <Divider />
        </Space>
      </Space>
      <div className="graph-container">
        {props.graphs.map((graphId) => (
          <Graph
            key={graphId}
            id={graphId}
            url={props.url}
            sync={sync}
            zoomData={zoomData}
            setZoomData={setZoomData}
            onDelete={() => props.deleteGraph(graphId, false)}
          />
        ))}
        {props.liveGraphs.map((graphId) => (
          <LiveGraph
            key={graphId}
            id={graphId}
            url={props.url}
            sync={sync}
            zoomData={zoomData}
            setZoomData={setZoomData}
            onDelete={() => props.deleteGraph(graphId, true)}
          />
        ))}
      </div>
    </div>
  );
}
