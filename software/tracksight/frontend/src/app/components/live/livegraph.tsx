import React, { useState, useEffect } from 'react';
import Plot from 'react-plotly.js';
import { Button, Card } from 'antd';

import LiveDropdownMenu from './dropdown_live.tsx';
import WebSocketComponent from './web_socket.tsx';

const DEFAULT_LAYOUT = {
    width: 620,
    height: 500,
    title: "Empty",
    xaxis: {},
    yaxis: {},
    legend: { "orientation": "h" },
}

const LiveGraph = (props) => {
    const [data, setData] = useState({});
    const [formattedData, setFormattedData] = useState([]);
    const [signals, setSignals] = useState([]);
    const [avail, setAvail] = useState([]);
    const [graphLayout, setGraphLayout] = useState(DEFAULT_LAYOUT);

    const getRandomColor = () => {
        const r = Math.floor(Math.random() * 256);
        const g = Math.floor(Math.random() * 256);
        const b = Math.floor(Math.random() * 256);
        return `rgb(${r},${g},${b})`;
    };

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

                for (let date in signalData) {
                    xData.push(date);
                    yData.push(signalData[date]);
                }

                const formattedObj = {
                    x: xData,
                    y: yData,
                    type: 'scatter',
                    mode: 'lines+markers',
                    name: signalName,
                    line: { color: getRandomColor() }
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
