import React, { useState, ReactElement } from 'react';
import { Divider, Button, Switch, Space } from 'antd';
import Graph from './db/graph';
import LiveGraph from './live/livegraph';
import { PlotRelayoutEvent } from 'plotly.js';
import { Socket } from "socket.io-client";
import { MessageInstance } from 'antd/es/message/interface';

export interface VisualizeProps {
    addGraph: (live: boolean) => void;
    graphs: number[];
    liveGraphs: number[];
    url: string;
    socket: Socket; 
    deleteGraph: (graphId: number, live: boolean) => void;
    messageApi: MessageInstance,
}

const Visualize = (props: VisualizeProps) => {
    const [sync, setSync] = useState<boolean>(false);
    const [zoomData, setZoomData] = useState<PlotRelayoutEvent>({});

    const changeSync = (checked: boolean) => {
        setSync(checked);
    };

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
                    <Button onClick={() => props.addGraph(false)}>Add Influx Graph</Button>
                    <Button onClick={() => props.addGraph(true)}>Add Live Graph</Button>
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
                        messageApi={props.messageApi}
                    />
                ))}
                {props.liveGraphs.map((graphId) => (
                    <LiveGraph
                        key={graphId}
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