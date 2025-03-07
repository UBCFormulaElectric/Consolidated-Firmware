import React, {
  useState,
  useEffect,
  Dispatch,
  MouseEventHandler,
  SetStateAction,
} from "react";
import dynamic from "next/dynamic";
import { PlotRelayoutEvent } from "plotly.js";
const Plot = dynamic(() => import("react-plotly.js"), { ssr: false });
import { Button, Card, Space } from "antd";
import { MessageInstance } from "antd/es/message/interface";

import QueryLive from "./query_live";

const DEFAULT_LAYOUT: Partial<Plotly.Layout> = {
  width: 620,
  height: 500,
  title: "Empty",
  xaxis: {},
  yaxis: {},
  legend: { orientation: "h" },
};

export default function LiveGraph(props: {
  id: number;
  url: string;
  sync: boolean;
  setZoomData: Dispatch<SetStateAction<PlotRelayoutEvent>>;
  zoomData: PlotRelayoutEvent;
  onDelete: MouseEventHandler<HTMLElement>;
}) {
  const [data, setData] = useState<{
    [name: string]: { times: Array<string>; values: Array<number> };
  }>({});
  const [formattedData, setFormattedData] = useState<Plotly.Data[]>([]);
  const [graphLayout, setGraphLayout] =
    useState<Partial<Plotly.Layout>>(DEFAULT_LAYOUT);

  const clearData = () => {
    setFormattedData([]);
    setGraphLayout(DEFAULT_LAYOUT);
    setData({});
  };

  // randomizes colour for graph lines
  const getRandomColor = () => {
    const r = Math.floor(Math.random() * 256);
    const g = Math.floor(Math.random() * 256);
    const b = Math.floor(Math.random() * 256);
    return `rgb(${r},${g},${b})`;
  };

  useEffect(() => {
    const tempFormattedData: Plotly.Data[] = [];
    for (const name in data) {
      let signalData = data[name];
      let xData = signalData["times"];
      let yData = signalData["values"];

      const formattedObj: Plotly.Data = {
        x: xData,
        y: yData,
        type: "scatter",
        mode: "lines+markers",
        name: name,
        line: { color: getRandomColor() },
      };

      tempFormattedData.push(formattedObj);
    }

    const newGraphName = Object.keys(data).join(" + ");
    setGraphLayout((prevLayout) => ({
      ...prevLayout,
      title: newGraphName,
    }));

    setFormattedData(tempFormattedData);
  }, [data]);

  // updates graph layout when zoomed
  useEffect(() => {
    if (props.zoomData && "xaxis.range[0]" in props.zoomData) {
      const xaxisRange0 = props.zoomData["xaxis.range[0]"];
      const xaxisRange1 = props.zoomData["xaxis.range[1]"];
      const yaxisRange0 = props.zoomData["yaxis.range[0]"];
      const yaxisRange1 = props.zoomData["yaxis.range[1]"];

      // Update the graph's layout with the new axis ranges
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

  const handleZoom = (e: Readonly<PlotRelayoutEvent>) => {
    if (props.sync) {
      props.setZoomData(e);
    }
  };

  return (
    // antd card takes this weird object, tailwind doesn't work on it
    <Card bodyStyle={{ display: "flex", flexDirection: "column" }}>
      <QueryLive url={props.url} setData={setData}></QueryLive>
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
      <Space.Compact size={"middle"}>
        <Button block={true} className="clear" onClick={clearData}>
          Clear
        </Button>
        <Button
          block={true}
          danger={true}
          ghost={false}
          onClick={props.onDelete}
        >
          Delete This Graph
        </Button>
      </Space.Compact>
    </Card>
  );
}
