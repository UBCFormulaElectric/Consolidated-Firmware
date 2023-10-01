'use client';

import { useState, useEffect, React } from 'react';
import Plot from 'react-plotly.js';

import WebSocketComponent from './web_socket.tsx';

const Graph = (props) => {
    
    const [data, setData] = useState({4: 1, 2: 3, 1:2});

const mockData = [{
    x: [1,2,3],
    y: [2,4,5],
    type: 'scatter',
    mode: 'lines+markers',
    name: "signal1" 
}, {
    x: [-1, 0 ,1],
    y: [2, 3, 4],
    type: 'scatter',
    mode: 'lines+markers',
    name: "signal2" 
},
{
    x: [2, 3, 4],
    y: [0, 1, 2],
    type: 'scatter',
    mode: 'lines+markers',
    name: "signal3" 
}
];

const [formattedData, setFormattedData] = useState([
    {
        x: [1,2,3],
        y: [2,4,5],
        type: 'scatter',
        mode: 'lines+markers',
        name: "signal1" 
    }, {
        x: [-1, 0 ,1],
        y: [2, 3, 4],
        type: 'scatter',
        mode: 'lines+markers',
        name: "signal2" 
    },
    {
        x: [2, 3, 4],
        y: [0, 1, 2],
        type: 'scatter',
        mode: 'lines+markers',
        name: "signal3" 
    },
]);

useEffect(() => {
    // Check if data is not empty
    if (Object.keys(data).length !== 0) {
        // Initialize an empty array to hold the new formatted data
        let newFormattedData = [...formattedData]; // Copy existing formattedData

        const currData = data;
        let xData = [];
        let yData = [];

        // Populate x and y arrays
        for (let k in currData) {
            let v = currData[k];
            xData.push(k);
            yData.push(v);
        }

        // Create a formatted data object for this signal
        const formattedObj = {
            x: xData,
            y: yData,
            type: 'scatter',
            mode: 'lines+markers',
            name: "signal5" 
        };

        // Add this formatted object to the newFormattedData array
        newFormattedData.push(formattedObj);

        // Update the state with the new formatted data
        setFormattedData(newFormattedData);
    }
}, [data]); // Added formattedData as a dependency


    return (
        <div>
            <WebSocketComponent socket={props.socket} setData={setData} />
            <Plot
                data={formattedData} // Pass the array of formatted data objects
                layout={{ width: "50%", height: "50%", title: 'Sample signal' }}
            />
        </div>
    );
}

export default Graph;
