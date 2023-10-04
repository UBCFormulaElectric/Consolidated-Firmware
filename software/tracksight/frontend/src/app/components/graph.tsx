'use client';

import { useState, useEffect, React } from 'react';
import Plot from 'react-plotly.js';
import DropdownMenu from './dropdown_menu.tsx';

const Graph = (props) => {
    
    const [data, setData] = useState({});
    const [formattedData, setFormattedData] = useState([]);
    const [graphName, setGraphName] = useState("Empty");

    const updateData = (newData) => {
        setData(oldData => {
            return {...oldData, ...newData};
        });
    }

    useEffect(() => {
        // Check if data is not empty
        if (Object.keys(data).length !== 0) {
            // Initialize an empty array to hold the new formatted data
            let newFormattedData = []; // Empty the array, we'll populate it based on the new data
            
            // Initialize an empty graph name, we'll populate it based on the new data
            let newGraphName = "";

            // Iterate through each signal in the data object
            for (let signalName in data) {
                let signalData = data[signalName];
    
                let xData = [];
                let yData = [];
    
                // Populate x and y arrays based on the signal's data
                for (let date in signalData) {
                    xData.push(date);
                    yData.push(signalData[date]);
                }
    
                // Create a formatted data object for this signal
                const formattedObj = {
                    x: xData,
                    y: yData,
                    type: 'scatter',
                    mode: 'lines+markers',
                    name: signalName
                };

                newGraphName += signalName + " + "; // Build the name dynamically
                newFormattedData.push(formattedObj);
            }
            
            // Remove trailing " + " from graph name
            newGraphName = newGraphName.slice(0, -3);

            // Update the states with the new formatted data and graph name
            setFormattedData(newFormattedData);
            setGraphName(newGraphName);
        }
    }, [data]);  // removed formattedData as a dependency to avoid infinite loop

    return (
        <div>
            <DropdownMenu socket={props.socket} setData={updateData} />
            <Plot
                data={formattedData} // Pass the array of formatted data objects
                layout={{ width: "50%", height: "50%", title: graphName }}
            />
        </div>
    );
}

export default Graph;

