import React, { useState, useEffect, Dispatch, MouseEventHandler, SetStateAction } from 'react';
import dynamic from "next/dynamic";
const Plot = dynamic(() => import("react-plotly.js"), { ssr: false, })
import { Button, Card, Switch, Space } from 'antd';

import LiveDropdownMenu from './dropdown_live';
import WebSocketComponent from './web_socket';
import { Socket } from "socket.io-client";

interface FormattedData {
    x: Date[];
    y: number[];
    type: string;
    mode: string;
    name: string;
}

interface Data {
    id: number;
    signals: Record<string, Record<string, number>>;
}


const DEFAULT_LAYOUT = {
    width: 620,
    height: 500,
    title: "Live Data",
    xaxis: {
        // for formatting time
        tickformat: "%H:%M:%S.%L",
        automargin: true,
    },
    yaxis: {},
    legend: { "orientation": "h" },
};

const MAX_DATA_POINTS = 100; // max amount of data points on the graph at a time
const UPDATE_INTERVAL_MS = 1000; // how often the graph updates
export interface LiveGraphProps {
    updateGraphSignals: any;
    id: number,
    onDelete: MouseEventHandler<HTMLElement>,
    socket: Socket,
    signals?: string[],
}
const LiveGraph = (props: LiveGraphProps) => {
    const [data, setData] = useState<Data>({ id: 0, signals: {} });
    const [formattedData, setFormattedData] = useState<FormattedData[]>([]);
    const [signals, setSignals] = useState<string[]>(props.signals || []);
    const [avail, setAvail] = useState<string[]>([]);
    const [graphLayout, setGraphLayout] = useState(DEFAULT_LAYOUT);

    // for live data simulation, remove when live signals are actually implemented
    const [useLive, setUseLive] = useState<boolean>(false);

    const changeLive = (checked: boolean) => {
        setUseLive(checked);
    }

    const clearData = () => {
        setFormattedData([]);
        setGraphLayout(DEFAULT_LAYOUT);
        setData({ id: 0, signals: {} });
        setSignals([]);
    }

    const formatData = () => {
        console.log(data['id']);
        if (data['id'] === props.id && data['signals']) {
            const signals = data['signals'];
            const newFormattedData: FormattedData[] = [];
    
            for (const signalName in signals) {
                const signalData = signals[signalName];
                const xData: Date[] = [];
                const yData: number[] = [];

                //extracts the most recent 100 data points to display to ensure the graph doesn't get too cluttered
                const sortedDates = Object.keys(signalData).sort((a, b) => new Date(a).getTime() - new Date(b).getTime());
                const maxDataPoints = sortedDates.slice(-MAX_DATA_POINTS);

                for (const date of maxDataPoints) {
                    xData.push(new Date(date));
                    yData.push(signalData[date]);
                }
    
                const formattedObj: FormattedData = {
                    x: xData, 
                    y: yData,
                    type: 'scatter',
                    mode: 'lines+markers',
                    name: signalName,
                };
    
                newFormattedData.push(formattedObj);
            }
    
            const newGraphName = Object.keys(signals).join(" + ");
            setGraphLayout((prevLayout) => ({
                ...prevLayout,
                title: newGraphName,
            }));
            setFormattedData(newFormattedData);
        }
    };
    

    useEffect(() => {
        formatData();
    }, [data]);

    useEffect(() => {
        props.socket.emit("available_signals", { "ids": [] });

        props.socket.on("available_signals_response", (signalNames) => {
            setAvail(signalNames);
        });

        return () => {
            props.socket.off("available_signals_response");
        };
    }, [props.socket]);

    useEffect(() => {
        props.updateGraphSignals(props.id, signals);
    }, [signals]);


    // for testing purposes only. uncomment to see live data simulation
    /*
    useEffect(() => {
        if (useLive) {
            const interval = setInterval(() => {
                const time = new Date().toISOString();
                const value = Math.random() * 0.5;

                // Update the data state with the new point
                setData((prevData) => {
                    let updatedData = { ...prevData };
                    updatedData = { ...updatedData['signals'] };
                    // Update the signal you want to mimic live data for
                    updatedData['LiveTest'] = {
                        ...updatedData['LiveTest'],
                        [time]: value,
                    };
                    const ret = {'id': props.id, 'signals': updatedData};
                    console.log(ret);
                    return ret;
                });
            }, UPDATE_INTERVAL_MS);

            return () => {
                clearInterval(interval);
            };
        }
    }, [useLive]);
    */

    return (
        <Card bodyStyle={{ display: 'flex', flexDirection: 'column' }}>
            <Space direction={"vertical"} size={"small"}>
            <p>Turn live signal on/off</p>
            <Switch onChange={changeLive} checked={useLive} />
            <Space size={"middle"}>
            <LiveDropdownMenu setSignal={setSignals} signals={signals} avail={avail} />
            <WebSocketComponent id={props.id} socket={props.socket} setData={setData} signals={props.signals || signals}></WebSocketComponent>
            </Space>
            </Space>
            {/*data and layout data were having overload issues, so the "as any" is a temp fix.*/}
            <Plot data={formattedData as any} layout={graphLayout as any} />
            <br></br>
            <Space.Compact size={"middle"}>
      <Button block={true} className="clear" onClick={clearData}>Clear</Button>
      <Button block={true} danger={true} ghost={false} onClick={props.onDelete}>Delete This Graph</Button>
      </Space.Compact>
        </Card>
    );
}

export default LiveGraph;