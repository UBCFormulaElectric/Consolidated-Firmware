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
          <Plot
            data={[formattedData]}
            layout={ {width: "100%", height: "100%", title: 'Sample signal'} }
          />
        </div>
    );
}

export default Graph;
