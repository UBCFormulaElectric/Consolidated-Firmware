'use client';

import { useState, useEffect } from 'react';
import Plot from 'react-plotly.js';
import { Card, Button } from 'antd';

import QueryData from './query_data.tsx';

const Graph = (props) => {
    const [data, setData] = useState({});
    const [formattedData, setFormattedData] = useState([]);
    const [graphName, setGraphName] = useState("Empty");

    const getRandomColor = () => {
        const r = Math.floor(Math.random() * 256);
        const g = Math.floor(Math.random() * 256);
        const b = Math.floor(Math.random() * 256);
        return `rgb(${r},${g},${b})`;
    };

    const clearData = () => {
        setFormattedData([]);
        setGraphName("Empty");
        setData({});
    }

    useEffect(() => {
        setGraphName("Empty");
        const tempFormattedData = [];
        for (const name in data) {
            let signalData = data[name];
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
                name: name,
                line: { color: getRandomColor() }
            };

            tempFormattedData.push(formattedObj);
        }
        setFormattedData(tempFormattedData);
        setGraphName(Object.keys(data).join(" + "));
    }, [data]);
    

    return (
        <Card bodyStyle={{ display: 'flex', flexDirection: 'column' }}>
            <QueryData url={props.url} setData={setData}></QueryData>
            <Plot
                data={formattedData} // Pass the array of formatted data objects
                layout={{ width: 650, height: 500, title: graphName }}
            />
            <br></br>
            <Button onClick={clearData}>Clear</Button>
            <Button onClick={props.onDelete}>Delete This Graph</Button>
        </Card>
    );
}

export default Graph;

