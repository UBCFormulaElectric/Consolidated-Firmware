import React, { useState, useEffect } from "react";
import {
  AreaChart,
  Area,
  XAxis,
  YAxis,
  Tooltip,
  // ResponsiveContainer, // !!! Removed ResponsiveContainer !!!
} from "recharts";

interface DataPoint {
  time: number;
  [signalName: string]: number;
}

interface NumericalGraphProps {
  currentTime: number;
  numericalSignals: string[];
}

const NumericalGraph: React.FC<NumericalGraphProps> = ({
  currentTime,
  numericalSignals,
}) => {
  const [data, setData] = useState<DataPoint[]>([]);
  const updateInterval = 200; // Update every 200 milliseconds

  useEffect(() => {
    const newDataPoint: DataPoint = { time: currentTime };
    // currently generating random values for each numerical signal
    numericalSignals.forEach((signalName) => {
      newDataPoint[signalName] = Math.floor(Math.random() * 100);
    });
    // ERIK: KEEP forever -> removing filter
    setData((prevData) => [...prevData, newDataPoint]);
  }, [currentTime, numericalSignals]);

  const colors = ["#ff4d4f", "#ffa940", "#36cfc9", "#597ef7", "#73d13d"];

  const pixelPerDataPoint = 50;
  const chartWidth = Math.max(data.length * pixelPerDataPoint, 100);

  //Slider to adjust horizontal scaling factor
  const [scaleFactor, setScaleFactor] = useState("100");
  //Combines the chartWidth with the slider value
  const finalChartWidth = chartWidth * (parseInt(scaleFactor) / 100);
  const [chartHeight, setChartHeight] = useState("256");

  return (
    <div className="w-full h-64">
      <div className="ml-24 mt-4 text-xs flex gap-4">
        {numericalSignals.map((signalName) => (
          <div key={signalName} className="flex items-center mb-1">
            <div
              className="w-3 h-3 inline-block mr-2"
              style={{
                backgroundColor: colors[numericalSignals.indexOf(signalName)],
              }}
            ></div>
            <span className="text-gray-500">{signalName}</span>
          </div>
        ))}
      </div>
      <div className="ml-24 mb-2">
        <label htmlFor="scaleSlider" className="mr-2">
          Horizontal Scale (%):
        </label>
        <input
          id="scaleSlider"
          type="range"
          min="1"
          max="1000"
          value={scaleFactor}
          onChange={(e) => setScaleFactor(e.target.value)}
        />
        <span className="ml-2">{scaleFactor}%</span>
      </div>
      <div className="ml-24 mb-2">
        <label htmlFor="heightSlider" className="mr-2">
          Vertical Scale (px):
        </label>
        <input
          id="heightSlider"
          type="range"
          min="1"
          max="1000"
          value={chartHeight}
          onChange={(e) => setChartHeight(e.target.value)}
        />
        <span className="ml-2">{chartHeight}px</span>
      </div>
      <div
        style={{
          width: `${finalChartWidth}px`,
          height: `${chartHeight}px`,
          transition: "width 0.3s ease-out" // !!! CSS transition to smooth width changes !!!
        }}
      >
        <AreaChart
          width={finalChartWidth}
          height={parseInt(chartHeight)}
          data={data}
        >
          <defs>
            {numericalSignals.map((signalName, index) => (
              <linearGradient
                key={signalName}
                id={`color${signalName}`}
                x1="0"
                y1="0"
                x2="0"
                y2="1"
              >
                <stop
                  offset="5%"
                  stopColor={colors[index % colors.length]}
                  stopOpacity={0.8}
                />
                <stop
                  offset="95%"
                  stopColor={colors[index % colors.length]}
                  stopOpacity={0}
                />
              </linearGradient>
            ))}
          </defs>
          <XAxis
            dataKey="time"
            type="number"
            domain={["dataMin", "dataMax"]}
            hide
          />
          <YAxis />
          <Tooltip
            labelFormatter={(value) => new Date(value).toLocaleTimeString()}
            formatter={(value, name) => [`${value}`, `${name}`]}
          />
          {/* ERIK: removed animation */}
          {numericalSignals.map((signalName, index) => (
            <Area
              key={signalName}
              type="monotone"
              dataKey={signalName}
              stroke={colors[index % colors.length]}
              fillOpacity={1}
              fill={`url(#color${signalName})`}
              isAnimationActive={false}
              animationDuration={updateInterval}
            />
          ))}
        </AreaChart>
      </div>
    </div>
  );
};

export default NumericalGraph;