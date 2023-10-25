import React, { useState, useEffect } from 'react';
import Plot from 'react-plotly.js';
import { Card, Button, Switch } from 'antd';
import QueryData from './query_data.tsx';

const DEFAULT_LAYOUT = {
  width: 620,
  height: 500,
  title: "Empty",
  xaxis: {},
  yaxis: {},
};

// props = key (graph key), id (graph id), url(for http), socket(for socket), sync (determines if should sync), 
// setZoomData (sets zoom data), zoomData (zoom data), onDelete (deletes a graph),
const Graph = (props) => {
  const [data, setData] = useState({});
  const [live, setLive] = useState(false);
  const [formattedData, setFormattedData] = useState([]);
  const [graphLayout, setGraphLayout] = useState(DEFAULT_LAYOUT);

  // for random colour generation of graph lines 
  const getRandomColor = () => {
    const r = Math.floor(Math.random() * 256);
    const g = Math.floor(Math.random() * 256);
    const b = Math.floor(Math.random() * 256);
    return `rgb(${r},${g},${b})`;
  };

  const changeLive = (checked: boolean) => {
    setLive(checked);
    console.log(checked);
  }


  // reset graph data
  const clearData = () => {
    setFormattedData([]);
    setGraphLayout(DEFAULT_LAYOUT);
    setData({});
  };

  // format data for graph
  const formatData = () => {
    const tempFormattedData = [];
    for (const name in data) {
      const signalData = data[name];
      const xData = [];
      const yData = [];

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

    const newGraphName = Object.keys(data).join(" + ");
    setGraphLayout((prevLayout) => ({
      ...prevLayout,
      title: newGraphName,
    }));
    setFormattedData(tempFormattedData);
  };

  const handleZoom = (e) => {
    props.setZoomData(e);
  };

  useEffect(() => {
    formatData();
  }, [data]);

  // changes zoom data if sync is on
  useEffect(() => {
    if (props.sync && props.zoomData && 'xaxis.range[0]' in props.zoomData) {
      const { 'xaxis.range[0]': xaxisRange0, 'xaxis.range[1]': xaxisRange1, 'yaxis.range[0]': yaxisRange0, 'yaxis.range[1]': yaxisRange1 } = props.zoomData;
      setGraphLayout((prevLayout) => ({
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

  return (
    <Card bodyStyle={{ display: 'flex', flexDirection: 'column' }}>
        <div className="space-between">
      <QueryData url={props.url} socket={props.socket} live={live} setData={setData} />
      <div className="center-align">
        <p>Use Live Data</p>
      <Switch onChange={changeLive}></Switch>
      </div>

      </div>
      <Plot
        data={formattedData}
        layout={graphLayout}
        config={{
          displayModeBar: true,
          displaylogo: false,
          scrollZoom: true,
        }}
        onRelayout={handleZoom}
      />
      <br />

      <Button onClick={clearData}>Clear</Button>
      <Button onClick={props.onDelete}>Delete This Graph</Button>
    </Card>
  );
};

export default Graph;
