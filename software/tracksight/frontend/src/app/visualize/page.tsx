'use client';
import { Dispatch, SetStateAction, useState } from 'react';
import { Button, Switch } from 'antd';
import Graph from '../components/db/graph';
import LiveGraph from '../components/live/livegraph';
import { PlotRelayoutEvent } from 'plotly.js';
import { GraphI } from '@/types/Graph';

function addGraph(setGraphFunc: Dispatch<SetStateAction<GraphI[]>>) {
    const newGraphId = Date.now();
    setGraphFunc(prevGraphs => [...prevGraphs, {
        id: newGraphId,
        timestamp: new Date()
    }]);
};

//delete a graph
function deleteGraph(setGraphFunc: Dispatch<SetStateAction<GraphI[]>>, gid: number) {
    setGraphFunc(prevGraphs => prevGraphs.filter(graph => graph.id !== gid));
};

export default function Visualize() {
    // const [graphSignals, setGraphSignals] = useState<Record<string, string[]>>({});
    // const [dbName, setDbName] = useState<string>("test");
    // const [modalOpen, setModalOpen] = useState<boolean>(false);

    // const updateGraphSignals = (graphId: number, signals: string) => {
    //     setGraphSignals(prev => ({ ...prev, [graphId]: signals }));
    // };

    // const handleInputChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    //     setDbName(event.target.value);
    // }

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


    const [sync, setSync] = useState<boolean>(false);
    const [zoomData, setZoomData] = useState<PlotRelayoutEvent>({});
    const [graphs, setGraphs] = useState<GraphI[]>([]);
    const [liveGraphs, setLiveGraphs] = useState<GraphI[]>([]);

    return (
        <div id="visualize-page" className="p-8">
            <div>
                <div>
                    <h1 className="text-4xl font-bold">Visualize</h1>
                    <p>Select a graph to provide live data or data from InfluxDB.</p>
                </div>
                <div className="flex flex-row gap-x-2 items-center">
                    <p>Sync Zoom</p>
                    <Switch onChange={(checked: boolean) => setSync(checked)} checked={sync} />
                    <Button onClick={(e) => addGraph(setGraphs)}>Add Influx Graph</Button>
                    <Button onClick={(e) => addGraph(setLiveGraphs)}>Add Live Graph</Button>
                    {/* TODO reenable modal */}
                    {/* <Button onClick={() => setModalOpen(true)}>Save Current Loadout</Button>
                    <Modal open={modalOpen} closeIcon={false} title='Save Dashboard'
                        cancelButtonProps={{ onClick: () => setModalOpen(false) }}
                        okButtonProps={{ onClick: () => saveDashboard() }}>
                        <Input placeholder='Name of dashboard...' onChange={handleInputChange}></Input>
                    </Modal> */}
                </div>

            </div>

            <div id="graph-container" className="flex flex-wrap gap-10 mt-6">
                {graphs.map((graph) => (
                    <Graph
                        key={graph.id}
                        graphInfo={graph}
                        onDelete={(e) => deleteGraph(setGraphs, graph.id)}
                        sync={sync}
                        sharedZoomData={zoomData}
                        setSharedZoomData={setZoomData}
                    />
                ))}
                {liveGraphs.map((liveGraph) => (
                    <LiveGraph
                        key={liveGraph.id}
                        id={liveGraph.id}
                        onDelete={(e) => deleteGraph(setLiveGraphs, liveGraph.id)}
                    // updateGraphSignals={updateGraphSignals}
                    // socket={props.socket}
                    />
                ))}
            </div>
        </div>
    );
};