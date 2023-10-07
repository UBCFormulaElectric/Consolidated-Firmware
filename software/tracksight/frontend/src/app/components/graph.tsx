'use client';

import { useState, useEffect, React } from 'react';
import Plot from 'react-plotly.js';
import DropdownMenu from './dropdown_menu.tsx';
import { Button } from 'antd';
import WebSocketComponent from './web_socket.tsx';

const Graph = (props) => {
    
    const [data, setData] = useState({});
    const [formattedData, setFormattedData] = useState([]);
    const [graphName, setGraphName] = useState("Empty");
    const [signal, setSignal] = useState("");
    // sets available signals 
    const [avail, setAvail] = useState([]);
    // adds a signal to a graph 
    const updateData = (newData) => {
        setData(oldData => {
            return {...oldData, ...newData};
        });
    }

    //resets a graphs data
    const clearData = () => {
        setFormattedData([]);
        setGraphName("Empty");
        setData({});
    }

    // Request available signals through socket on render
        useEffect(() => {
            props.socket.emit("available_signals", {"ids": []});
            
            // Listen for the server's response
            props.socket.on("available_signals_response", (signalNames) => {
                // Update the state with new data
                setAvail(signalNames);
            });
    
            // Cleanup
            return () => {
                props.socket.off("available_signals_response");
            };
        }, [props.socket]);


    useEffect(() => {
        // Check if data is not empty
        if (Object.keys(data).length !== 0) {
            if (signal === Object.keys(data)[0]) {
            // Initialize an empty array to hold the new formatted data
            let newFormattedData = []; 
            
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
    }
    }, [data]);  

    return (
        <div>
            <WebSocketComponent socket={props.socket} setData={setData} selectedSignal={signal}></WebSocketComponent>
            <DropdownMenu setSignal={setSignal} signals={avail}/>
            <Plot
                data={formattedData} // Pass the array of formatted data objects
                layout={{ width: 650, height: 500, title: graphName }}
            />
            <br></br>
            <Button onClick={clearData}>Clear</Button>
        </div>
    );
}

export default Graph;

