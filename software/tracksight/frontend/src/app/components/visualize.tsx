import React from 'react';
import { Divider, Button, Switch } from 'antd';
import Graph from './graph.tsx';

const Visualize = ({ setSync, addGraph, graphs, sync, setZoomData, deleteGraph, zoomData, url, socket }) => {
    return (
        <div className="layout">
            <h1>Visualize</h1>
            <p>Select a signal from the dropdown menu and then press submit to visualize the data on the graph.</p>
            <div>
                <Switch onClick={setSync}></Switch>
                <p>Sync Zoom</p>
            </div>
            <Button onClick={addGraph}>Add Another Graph</Button>
            <Divider></Divider>
            <div className="flex-container">
                {graphs.map(graphId => (
                    <Graph
                        key={graphId} id={graphId} url={url} sync={sync} setZoomData={setZoomData} zoomData={zoomData}
                        onDelete={() => deleteGraph(graphId)}
                    />
                ))}
            </div>
        </div>
    );
}

export default Visualize;
