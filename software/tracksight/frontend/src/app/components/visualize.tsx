import React, { useState, ReactElement } from 'react';
import { Divider, Button, Switch, Space } from 'antd';
import { PlotRelayoutEvent } from 'plotly.js';
import { Socket } from "socket.io-client";
import { MessageInstance } from 'antd/es/message/interface';

import Graph from './db/graph';
import LiveGraph from './live/livegraph';
import TimeStampPicker from './db/timestamp_picker';

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
    const [startEpoch, setStartEpoch] = useState<string>("");
    const [endEpoch, setEndEpoch] = useState<string>("");

    return (
        <div className="layout">
            <Space direction="vertical" size="large">
                <Space size="middle" direction="horizontal">
                    <TimeStampPicker setStart={setStartEpoch} setEnd={setEndEpoch} />
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
                        sync={true}
                        startEpoch={startEpoch}
                        endEpoch={endEpoch}
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
