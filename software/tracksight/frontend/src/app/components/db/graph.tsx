'use client';
import { useState, useEffect, Dispatch, MouseEventHandler, SetStateAction } from 'react';
import dynamic from "next/dynamic";
import { PlotRelayoutEvent } from 'plotly.js';
import { Card, Button, Space } from 'antd';
import { GraphI } from '@/types/Graph';
import DropdownMenu from './dropdown_menu';
import TimeStampPicker from './timestamp_picker';
import { FLASK_URL } from '@/app/constants';

const Plot = dynamic(() => import("react-plotly.js"), { ssr: false, })

const DEFAULT_LAYOUT: Partial<Plotly.Layout> = {
    width: 620,
    height: 500,
    title: "Empty",
    xaxis: {},
    yaxis: {},
    legend: { "orientation": "h" },
}

const getRandomColor = () => {
    const r = Math.floor(Math.random() * 256);
    const g = Math.floor(Math.random() * 256);
    const b = Math.floor(Math.random() * 256);
    return `rgb(${r},${g},${b})`;
};

const Graph = (props: {
    graphInfo: GraphI,
    sync: boolean,
    sharedZoomData: PlotRelayoutEvent,
    setSharedZoomData: Dispatch<SetStateAction<PlotRelayoutEvent>>
    onDelete: MouseEventHandler<HTMLInputElement>,
}) => {
    const [data, setData] = useState<Record<string, { times: Array<string>, values: Array<number> }>>({});
    const [formattedData, setFormattedData] = useState<Plotly.Data[]>([]);

    //default graph layout
    const [graphLayout, setGraphLayout] = useState<Partial<Plotly.Layout>>(DEFAULT_LAYOUT);

    // resets data on graph
    const clearData = () => {
        setFormattedData([]);
        setGraphLayout(DEFAULT_LAYOUT);
        setData({});
    }


    // creates a new graph with request signals
    // currently rerendering entire graph everytime there is zoom/change in signal. Not ideal in terms of performance, 
    // suggestions for improvements appreciated. 
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


    // updates graph layout when zoomed 
    useEffect(() => {
        if (props.sharedZoomData && 'xaxis.range[0]' in props.sharedZoomData) {
            const xaxisRange0 = props.sharedZoomData['xaxis.range[0]'];
            const xaxisRange1 = props.sharedZoomData['xaxis.range[1]'];
            const yaxisRange0 = props.sharedZoomData['yaxis.range[0]'];
            const yaxisRange1 = props.sharedZoomData['yaxis.range[1]'];

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
    }, [props.sharedZoomData]);


    const [measurement, setMeasurement] = useState<string[]>([]);
    const [allMeasurements, setAllMeasurements] = useState<string[]>([]);

    const [fields, setFields] = useState<string[]>([]);
    const [allFields, setAllFields] = useState<string[]>([]);

    const [startEpoch, setStartEpoch] = useState<string>("");
    const [endEpoch, setEndEpoch] = useState<string>("");

    useEffect(() => {
        fetch(FLASK_URL + "/signal/measurement", {
            method: 'get',
        }).then((response) => response.json())
            .then((data) => setAllMeasurements(data))
            .catch((error) => console.log(error));
    }, []);

    useEffect(() => {
        if (!measurement.length) {
            return;
        }

        const measurement_name = measurement[0];
        fetch(FLASK_URL + "/signal/fields/" + measurement_name, {
            method: 'get',
        }).then((response) => response.json())
            .then((data) => setAllFields(data))
            .catch((error) => console.log(error));
    }, [measurement]);


    return (
        <Card bodyStyle={{ display: 'flex', flexDirection: 'column' }}>
            <div className="flex flex-col">
                <DropdownMenu setOption={setMeasurement} selectedOptions={measurement} options={allMeasurements} single={true} name={"Measurements"} />
                <DropdownMenu setOption={setFields} selectedOptions={fields} options={allFields} single={false} name={"Fields"} />
                <TimeStampPicker setStart={setStartEpoch} setEnd={setEndEpoch} />
                <Button onClick={(e) => {
                    if (!startEpoch || !endEpoch || !measurement || !fields.length) {
                        // TODO add message
                        // props.messageApi.open({type: "error", content: "Please fill out all fields properly"});
                        return;
                    }
                    const newParams = new URLSearchParams({ measurement: measurement[0], start_epoch: startEpoch, end_epoch: endEpoch });
                    for (const field in fields) {
                        newParams.append('fields', fields[field]);
                    }
                    fetch(FLASK_URL + "/query?" + newParams)
                        .then((response) => {
                            if (!response.ok) {
                                return response.json().then(json => { throw new Error(json["error"]) });
                            } else {
                                return response.json()
                            }
                        })
                        .then((data) => props.setData(data))
                        .catch((error) => { })// TODO props.messageApi.open({type: "error", content: error.toString()}));
                }}>
                    submit
                </Button>
            </div>
            <Plot
                data={formattedData} // Pass the array of formatted data objects
                layout={graphLayout}
                config={{
                    displayModeBar: true,
                    displaylogo: false,
                    scrollZoom: true,
                }}
                onRelayout={(e: Readonly<PlotRelayoutEvent>) => {
                    if (props.sync) props.setSharedZoomData(e);
                }}
            />
            <Space.Compact size={"middle"}>
                <Button block={true} className="clear" onClick={clearData}>Clear</Button>
                <Button block={true} danger={true} ghost={false} onClick={props.onDelete}>Delete This Graph</Button>
            </Space.Compact>
        </Card>
    );
}

export default Graph;