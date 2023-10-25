import React from 'react';
import { useState } from 'react';
import { Divider, Button, Switch } from 'antd';
import Graph from './graph.tsx';

const Visualize = ({ addGraph, graphs, deleteGraph, url, socket }) => {

    // determines if all graphs are supposed to zoom together or not
    const [sync, setSync] = useState(false);
    const [zoomData, setZoomData] = useState([]);

    const changeSync = (checked: boolean) => {
        setSync(checked);
    }
    return (
        <div className="layout">
            <h1>Visualize</h1>
            <p>Select a signal from the dropdown menu and then press submit to visualize the data on the graph.</p>
            <div>
                <Switch onChange={changeSync}></Switch>
                <p>Sync Zoom</p>
            </div>
            <Button onClick={addGraph}>Add Another Graph</Button>
            <Divider></Divider>
            <div className="flex-container">
                {graphs.map(graphId => (
                    <Graph
                        key={graphId} 
                        id={graphId}
                        url={url} 
                        sync={sync}
                        setZoomData={setZoomData} 
                        zoomData={zoomData}
                        onDelete={() => deleteGraph(graphId)} 
                        socket={socket}
                    />
                ))}
            </div>
        </div>
    );
}

export default Visualize;
