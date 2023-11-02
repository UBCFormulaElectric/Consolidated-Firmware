import React, { useState, useEffect } from 'react';
import Plot from 'react-plotly.js';
import { Button, Card } from 'antd';

import LiveDropdownMenu from './dropdown_live.tsx';
import WebSocketComponent from './web_socket.tsx';

const DEFAULT_LAYOUT = {
    width: 620,
    height: 500,
    title: "Live Data",
    xaxis: {
        // for formatting time 
        tickformat: "%H:%M:%S.%L", 
    },
    yaxis: {},
    legend: { "orientation": "h" },
};

const UPDATE_INTERVAL_MS = 100; // changes update spped
const MAX_DATA_POINTS = 100; // max amount of data points on graph at a time 

const LiveGraph = (props) => {
    const [data, setData] = useState({});
    const [formattedData, setFormattedData] = useState([]);
    const [signals, setSignals] = useState([]);
    const [avail, setAvail] = useState([]);
    const [graphLayout, setGraphLayout] = useState(DEFAULT_LAYOUT);

    const clearData = () => {
        setFormattedData([]);
        setGraphLayout(DEFAULT_LAYOUT);
        setData({});
        setSignals([]);
    }

    const formatData = () => {
        let len = Object.keys(data).length;
        if (len !== 0) {
            let newFormattedData = [];

            for (let signalName in data) {
                let signalData = data[signalName];
                let xData = [];
                let yData = [];

                const sortedDates = Object.keys(signalData).sort((a, b) => new Date(a) - new Date(b));
                const lastNItems = sortedDates.slice(-MAX_DATA_POINTS);

                for (let date of lastNItems) {
                    xData.push(new Date(date)); 
                    yData.push(signalData[date]);
                }

                const formattedObj = {
                    x: xData,
                    y: yData,
                    type: 'scatter',
                    mode: 'lines+markers',
                    name: signalName,
                };

                newFormattedData.push(formattedObj);
            }

            const newGraphName = Object.keys(data).join(" + ");
            setGraphLayout((prevLayout) => ({
                ...prevLayout,
                title: newGraphName,
            }));
            setFormattedData(newFormattedData);
        }
    }

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

    // for live data simulation 

    useEffect(() => {
        const interval = setInterval(() => {
            const time = new Date().toISOString(); 
            const value = Math.random() * 0.5; 

            // Update the data state with the new point
            setData((prevData) => {
                const updatedData = { ...prevData };
                // Update the signal you want to mimic live data for
                updatedData['LiveTest'] = {
                    ...updatedData['LiveTest'],
                    [time]: value,
                };
                return updatedData;
            });
        }, UPDATE_INTERVAL_MS);

        return () => {
            clearInterval(interval);
        };
    }, []);

    return (
        <Card bodyStyle={{ display: 'flex', flexDirection: 'column' }}>
            <LiveDropdownMenu setSignal={setSignals} signals={signals} avail={avail} />
            <WebSocketComponent socket={props.socket} setData={setData} signals={signals}></WebSocketComponent>
            <Plot
                data={formattedData}
                layout={graphLayout}
            />
            <br></br>
            <Button onClick={clearData}>Clear</Button>
            <Button onClick={props.onDelete}>Delete This Graph</Button>
        </Card>
    );
}

export default LiveGraph;
