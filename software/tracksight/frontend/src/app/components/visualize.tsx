import React from 'react';
import { useState } from 'react';
import { Divider, Button, Switch, Space } from 'antd';
import Graph from './db/graph'
import LiveGraph from './live/livegraph';

const Visualize = (props) => {
    // Determines if all graphs are supposed to zoom together or not
    const [sync, setSync] = useState(false);
    const [zoomData, setZoomData] = useState([]);

    const changeSync = (checked) => {
        setSync(checked);
    }

    // TODO -> fix graph order behaviour, i.e make it so that you can move graphs around and live + regular graphs can 
    // be mixed and matched. React DnD may be a good library to use for this

    return (
        <div className="layout">
            <Space direction={"vertical"} size={"large"}>
            <div>
            <h1>Visualize</h1>
            <p>Select a graph to provide live data or data from InfluxDB.</p>
            </div>
            <Space size={"middle"}>
                <Space direction={"vertical"}  size={"small"}>
                <p>Sync Zoom</p>
                <Switch onChange={changeSync} checked={sync}></Switch>
                </Space>
                <Button onClick={() => props.addGraph(false)}>Add Influx Graph</Button>
                <Button onClick={() => props.addGraph(true)}>Add Live Graph</Button>
            <Divider></Divider>
            </Space>
            </Space>
            <div className="graph-container">
            {props.graphs.map(graphId => (
                  <Graph
                  id={graphId}
                  url={props.url}
                  sync={sync}
                  zoomData={zoomData}
                  setZoomData={setZoomData}
                  onDelete={() => props.deleteGraph(graphId, false)}
              />
            ))}
            {props.liveGraphs.map(graphId => (
                    <LiveGraph
                    id={graphId}
                    onDelete={() => props.deleteGraph(graphId, true)}
                    socket={props.socket}
                />
            ))}
            </div>
        </div>
    );
};

export default Visualize;
