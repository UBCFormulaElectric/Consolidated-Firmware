import React, { useState, useEffect, Dispatch, MouseEventHandler, SetStateAction } from 'react';
import dynamic from "next/dynamic";
import { PlotRelayoutEvent } from 'plotly.js';
const Plot = dynamic(() => import("react-plotly.js"), { ssr: false, })
import { Button, Card, Space } from 'antd';
import { MessageInstance } from 'antd/es/message/interface';

import QueryLive from './query_live'

const DEFAULT_LAYOUT: Partial<Plotly.Layout> = {
    width: 620,
    height: 500,
    title: "Empty",
    xaxis: {},
    yaxis: {},
    legend: { "orientation": "h" },
}

export interface LiveGraphProps {
    url: string,
    id: number,
    messageApi: MessageInstance,
    updateGraphSignals: any;
    onDelete: MouseEventHandler<HTMLElement>,
    signals?: string[],
}

const LiveGraph = (props: LiveGraphProps) => {
    const [data, setData] = useState<{ [name: string]: { times: Array<string>, values: Array<number> } }>({});
    const [formattedData, setFormattedData] = useState<Plotly.Data[]>([]);

    const [signals, setSignals] = useState<string[]>(props.signals || []);
    const [graphLayout, setGraphLayout] = useState<Partial<Plotly.Layout>>(DEFAULT_LAYOUT);

    const clearData = () => {
        setFormattedData([]);
        setGraphLayout(DEFAULT_LAYOUT);
        setData({});
        setSignals([]);
    }

    useEffect(() => {
        const tempFormattedData: Plotly.Data[] = [];
        for (const name in data) {
            let signalData = data[name];
            let xData = signalData["times"];
            let yData = signalData["values"];

            const formattedObj: Plotly.Data = {
                x: xData,
                y: yData,
                type: 'scatter',
                mode: 'lines+markers',
                name: name,
                // line: { color: getRandomColor() }
            };

            tempFormattedData.push(formattedObj);
        }

        const newGraphName = Object.keys(data).join(" + ");
        setGraphLayout(prevLayout => ({
            ...prevLayout,
            title: newGraphName,
        }));

        setFormattedData(tempFormattedData);
    }, [data]);

    useEffect(() => {
        props.updateGraphSignals(props.id, signals);
    }, [signals]);


    // useEffect(() => {
    //     if (useLive) {
    //         const interval = setInterval(() => {
    //             const time = new Date().toISOString();
    //             const value = Math.random() * 0.5;

    //             // Update the data state with the new point
    //             setData((prevData) => {
    //                 let updatedData = { ...prevData };
    //                 updatedData = { ...updatedData['signals'] };
    //                 // Update the signal you want to mimic live data for
    //                 updatedData['LiveTest'] = {
    //                     ...updatedData['LiveTest'],
    //                     [time]: value,
    //                 };
    //                 const ret = { 'id': props.id, 'signals': updatedData };
    //                 console.log(ret);
    //                 return ret;
    //             });
    //         }, UPDATE_INTERVAL_MS);

    //         return () => {
    //             clearInterval(interval);
    //         };
    //     }
    // }, [useLive]);

    // updates graph layout when zoomed 
    useEffect(() => {
        if (props.zoomData && 'xaxis.range[0]' in props.zoomData) {
            const xaxisRange0 = props.zoomData['xaxis.range[0]'];
            const xaxisRange1 = props.zoomData['xaxis.range[1]'];
            const yaxisRange0 = props.zoomData['yaxis.range[0]'];
            const yaxisRange1 = props.zoomData['yaxis.range[1]'];

            // Update the graph's layout with the new axis ranges
            setGraphLayout(prevLayout => ({
                ...prevLayout,
                xaxis: {
                    range: [xaxisRange0, xaxisRange1],
                },
                yaxis: {
                    range: [yaxisRange0, yaxisRange1],
                },
            }));
        }
    }, [props.zoomData]);

    const handleZoom = (e: Readonly<PlotRelayoutEvent>) => {
        if (props.sync) {
            props.setZoomData(e);
        }
    }

    return (
        <Card bodyStyle={{ display: 'flex', flexDirection: 'column' }}>
            <QueryLive url={props.url} setData={setData} messageApi={props.messageApi}></QueryLive>
            <Plot
                data={formattedData} // Pass the array of formatted data objects
                layout={graphLayout}
                config={{
                    displayModeBar: true,
                    displaylogo: false,
                    scrollZoom: true,
                }}
                onRelayout={handleZoom}
            />
            <br></br>
            <Space.Compact size={"middle"}>
                <Button block={true} className="clear" onClick={clearData}>Clear</Button>
                <Button block={true} danger={true} ghost={false} onClick={props.onDelete}>Delete This Graph</Button>
            </Space.Compact>
        </Card>
    );
}

export default LiveGraph;