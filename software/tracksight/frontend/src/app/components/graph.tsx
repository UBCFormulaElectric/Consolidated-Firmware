'use client';

import { useState, useEffect } from 'react';
import Plot from 'react-plotly.js';
import { Card, Button } from 'antd';

import QueryData from './query_data.tsx';

const DEFAULT_LAYOUT = {
    width: 620, 
    height: 500, 
    title: "Empty",
    xaxis: {},
    yaxis: {},
}

const Graph = (props) => {
    const [data, setData] = useState({});
    const [formattedData, setFormattedData] = useState([]);

    //default graph layout
    const [graphLayout, setGraphLayout] = useState(DEFAULT_LAYOUT);

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


    // creates a new graph with request signals
    // currently rerendering entire graph everytime there is zoom/change in signal. Not ideal in terms of performance, 
    // suggestions for improvements appreciated. 
    useEffect(() => {
        const tempFormattedData = [];
        for (const name in data) {
            let signalData = data[name];
            let xData = signalData["time"];
            let yData = signalData["value"];

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

        const newGraphName = Object.keys(data).join(" + ");

        setGraphLayout(prevLayout => ({
        ...prevLayout,
        title: newGraphName,
       }));
        setFormattedData(tempFormattedData);
    }, [data]);


    // updates graph layout when zoomed 
    useEffect(() => {
        if (props.sync && props.zoomData && 'xaxis.range[0]' in props.zoomData) {
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

    const handleZoom = (e) => {
        props.setZoomData(e);
    }


    return (
        <Card
        bodyStyle={{ display: 'flex', flexDirection: 'column' }}>
            <QueryData url={props.url} setData={setData}></QueryData>
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
            <Button onClick={clearData}>Clear</Button>
            <Button onClick={props.onDelete}>Delete This Graph</Button>
        </Card>
    );
}

export default Graph;

