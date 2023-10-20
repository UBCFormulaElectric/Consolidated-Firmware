'use client';

import { useState, useEffect, useRef } from 'react';
import Plot from 'react-plotly.js';
import { Card, Button } from 'antd';

import QueryData from './query_data.tsx';

const Graph = (props) => {
    const [data, setData] = useState({});
    const [formattedData, setFormattedData] = useState([]);
    const [graphName, setGraphName] = useState("Empty");
    
    //default graph layout
    const [graphLayout, setGraphLayout] = useState({
        width: 620, 
        height: 500, 
        title: graphName,
        xaxis: {},
        yaxis: {},
    });
    const graphDivRef = useRef(null);

    //layout for graph 
    const layout = {
        width: 620, 
        height: 500, 
        title: graphName,
        xaxis: {range: ["2017-04-04", "2017-08-06"]},
        yaxis: {range: ["9:00:00", "24:00:00"]},
     }


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
        setGraphName("Empty");
        setData({});
    }


    // creates a new graph with request signals
    // currently rerendering entire graph everytime there is zoom/change in signal. Not ideal in terms of performance, 
    // suggestions for improvements appreciated. 
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

    const handleZoom = (e) => {
        props.setZoomData(e);
    }


    return (
        <Card ref={graphDivRef}
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

