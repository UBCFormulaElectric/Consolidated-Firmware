'use client';

import { useState, useEffect, Dispatch, MouseEventHandler, SetStateAction } from 'react';
import dynamic from "next/dynamic";
const Plot = dynamic(() => import("react-plotly.js"), { ssr: false, })
import { PlotRelayoutEvent } from 'plotly.js';

import { Card, Button, Space } from 'antd';

import QueryData from './query_data';
import { MessageInstance } from 'antd/es/message/interface';

const DEFAULT_LAYOUT: Partial<Plotly.Layout> = {
    width: 620, 
    height: 500, 
    title: "Empty",
    xaxis: {},
    yaxis: {},
    legend: {"orientation": "h"},
}

export interface GraphProps {
    id: number,
    url: string,
    sync: boolean,
    startEpoch: string,
    endEpoch: string,
    onDelete: MouseEventHandler<HTMLElement>,
    messageApi: MessageInstance,
}

const Graph = (props: GraphProps) => {
    const [data, setData] = useState<{[name: string]: {time: Array<string>, value: Array<number>}}>({});
    const [formattedData, setFormattedData] = useState<Plotly.Data[]>([]);

    //default graph layout
    const [graphLayout, setGraphLayout] = useState<Partial<Plotly.Layout>>(DEFAULT_LAYOUT);

     // randomizes colour for graph lines 
    const getRandomColor = () => {
        const r = Math.floor(Math.random() * 256);
        const g = Math.floor(Math.random() * 256);
        const b = Math.floor(Math.random() * 256);
        return `rgb(${r},${g},${b})`;
    };

    // resets data on graph
    const clearData = () => {
        setFormattedData([]);
        setGraphLayout(DEFAULT_LAYOUT);
        setData({});
    }

    useEffect(() => {
        const tempFormattedData: Plotly.Data[] = [];
        for (const name in data) {
            let signalData = data[name];
            let xData = signalData["time"];
            let yData = signalData["value"];

            const formattedObj: Plotly.Data = {
                x: xData,
                y: yData,
                type: 'scatter',
                mode: 'lines+markers',
                name: name,
                line: { color: getRandomColor() }
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

    return (
        <Card
        bodyStyle={{ display: 'flex', flexDirection: 'column' }}>
            <QueryData startEpoch={props.startEpoch} endEpoch={props.endEpoch} url={props.url} setData={setData} messageApi={props.messageApi}></QueryData>
            <Plot
                data={formattedData} // Pass the array of formatted data objects
                layout={graphLayout}
                config={{
                    displayModeBar: true,
                    displaylogo: false,
                    scrollZoom: true,
                  }}
            />
            <br></br>
            <Space.Compact size={"middle"}>
      <Button block={true} className="clear" onClick={clearData}>Clear</Button>
      <Button block={true} danger={true} ghost={false} onClick={props.onDelete}>Delete This Graph</Button>
      </Space.Compact>
        </Card>
    );
}

export default Graph;
