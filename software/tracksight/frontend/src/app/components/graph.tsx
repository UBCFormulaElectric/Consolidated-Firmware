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
    const [signals, setSignals] = useState([])
    // sets available signals 
    const [avail, setAvail] = useState([]);
    // adds a signal to a graph 
    const updateData = (newData) => {
        setData(oldData => {
            return {...oldData, ...newData};
        });
    }

    const getRandomColor = () => {
        const r = Math.floor(Math.random() * 256);
        const g = Math.floor(Math.random() * 256);
        const b = Math.floor(Math.random() * 256);
        return `rgb(${r},${g},${b})`;
    };
    

    //resets a graphs data
    const clearData = () => {
        setFormattedData([]);
        setGraphName("Empty");
        setData({});
        setSignals([]);
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
            let len = Object.keys(data).length;
            if (len !== 0) {
                for (let i = 0; i < len; i++) {
                if (signals[i] === Object.keys(data)[i]) {
                    let newFormattedData = [];
                    let newGraphName = "";
        
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
                            line: { color: getRandomColor() } // Add this line
                        };
        
                        newGraphName += signalName + " + ";
                        newFormattedData.push(formattedObj);
                    }
        
                    newGraphName = newGraphName.slice(0, -3);
                    setFormattedData(newFormattedData);
                    setGraphName(newGraphName);
                }
            }
        }
        }, [data]);
        

    return (
        <div>
                        <WebSocketComponent socket={props.socket} setData={setData} signals={signals}></WebSocketComponent>
            <DropdownMenu setSignal={setSignals} signals={avail} />
            <Plot
                data={formattedData} // Pass the array of formatted data objects
                layout={{ width: 650, height: 500, title: graphName }}
            />
            <br></br>
            <Button onClick={clearData}>Clear</Button>
            <Button onClick={props.onDelete}>Delete This Graph</Button>
        </div>
    );
}

export default Graph;

