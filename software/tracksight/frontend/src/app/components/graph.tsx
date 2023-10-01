'use client';

import { useState, useEffect, React } from 'react';
import Plot from 'react-plotly.js';

import WebSocketComponent from './web_socket.tsx';


const Graph = (props) => {
	const [data, setData] = useState({});
    const [formattedData, setFormattedData] = useState({
        x: [],
        y: [],
        type: 'scatter',
        mode: 'lines+markers',
        marker: {color: 'red'},
    });

    
    useEffect(() => {
        // loads data from the websocket into the formattedData state
        if (Object.keys(data).length !== 0) {
            const currData = data["Signal1"];
            const newFormattedData = formattedData;

            for (let k in currData) {
                let v = currData[k];
                newFormattedData['x'].push(k);
                newFormattedData['y'].push(v);
            }
            setFormattedData(newFormattedData);
        }
    }, [data]);

    return (
        <div>
          <WebSocketComponent socket={props.socket} setData={setData}/>
          {/* each plot takes a data; starts with a preset state but once loaded plots new points by adding k/v to 
          the x/y arrays respectively */}
    
          <Plot
            data={[formattedData]}
            layout={ {width: "50%", height: "50%", title: 'Sample signal'} }
          />
          
        </div>
    );
}

export default Graph;
