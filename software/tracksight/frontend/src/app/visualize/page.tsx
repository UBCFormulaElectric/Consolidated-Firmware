'use client';
import { useState } from 'react';
import { Button, Switch } from 'antd';
import Graph from './graph';
import LiveGraph from './livegraph';
import { PlotRelayoutEvent } from 'plotly.js';
import { GraphI, GraphType } from '@/types/Graph';

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


    const [shouldSyncZoom, setShouldSyncZoom] = useState<boolean>(false);
    const [zoomData, setZoomData] = useState<PlotRelayoutEvent>({});
    const [graphs, setGraphs] = useState<GraphI[]>([]);


    function addGraph(graphType: GraphType) {
        setGraphs(prevGraphs => [...prevGraphs, {
            id: Date.now(),
            timestamp: new Date(),
            type: graphType
        }]);
    };

    //delete a graph
    function deleteGraph(gid: number) {
        setGraphs(prevGraphs => prevGraphs.filter(graph => graph.id !== gid));
    };

    return (
        <div id="visualize-page" className="p-8">
            <div>
                <div>
                    <h1 className="text-4xl font-bold">Visualize</h1>
                    <p>Select a graph to provide live data or data from InfluxDB.</p>
                </div>
                <div className="flex flex-row gap-x-2 items-center">
                    <p>Sync Zoom</p>
                    <Switch onChange={(checked: boolean) => setShouldSyncZoom(checked)} checked={shouldSyncZoom} />
                    <Button onClick={(e) => addGraph(GraphType.HISTORICAL)}>Add Influx Graph</Button>
                    <Button onClick={(e) => addGraph(GraphType.LIVE)}>Add Live Graph</Button>
                    {/* TODO reenable modal */}
                    {/* <Button onClick={() => setModalOpen(true)}>Save Current Loadout</Button>
                    <Modal open={modalOpen} closeIcon={false} title='Save Dashboard'
                        cancelButtonProps={{ onClick: () => setModalOpen(false) }}
                        okButtonProps={{ onClick: () => saveDashboard() }}>
                        <Input placeholder='Name of dashboard...' onChange={handleInputChange}></Input>
                    </Modal> */}
                </div>

            </div>

            <div id="graph-container" className="flex flex-wrap gap-4 mt-6">
                {graphs.map((graph) => {
                    if(graph.type == GraphType.HISTORICAL) {
                        return (
                            <Graph
                                key={graph.id}
                                graphInfo={graph}
                                handleDelete={(e) => deleteGraph(graph.id)}
                                syncZoom={shouldSyncZoom}
                                sharedZoomData={zoomData}
                                setSharedZoomData={setZoomData}
                            />
                        )
                    }
                    // TODO reconsider whether we need a different component for live and non live graph data
                    else if (graph.type == GraphType.LIVE) {
                        return (
                            <LiveGraph
                                key={graph.id}
                                id={graph.id}
                                onDelete={(e) => deleteGraph(graph.id)}
                                // updateGraphSignals={updateGraphSignals}
                                // socket={props.socket}
                            />
                        )
                    }
                    else {
                        throw new Error("Invalid graph type");
                    }
                })}
            </div>
        </div>
    );
};