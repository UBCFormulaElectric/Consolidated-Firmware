import React, { useState, useEffect } from "react";
import {
  AreaChart,
  Area,
  XAxis,
  YAxis,
  Tooltip,
  ResponsiveContainer,
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

    setData((prevData) => {
      const timeWindow = 30000; // 30 seconds in milliseconds
      const cutoffTime = currentTime - timeWindow;
      const filteredData = prevData.filter((d) => d.time >= cutoffTime);
      return [...filteredData, newDataPoint];
    });
  }, [currentTime, numericalSignals]);

  const colors = ["#ff4d4f", "#ffa940", "#36cfc9", "#597ef7", "#73d13d"];

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
      <ResponsiveContainer width="100%" height="100%">
        <AreaChart data={data}>
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
          {numericalSignals.map((signalName, index) => (
            <Area
              key={signalName}
              type="monotone"
              dataKey={signalName}
              stroke={colors[index % colors.length]}
              fillOpacity={1}
              fill={`url(#color${signalName})`}
              isAnimationActive={true}
              animationDuration={updateInterval}
            />
          ))}
        </AreaChart>
      </ResponsiveContainer>
    </div>
  );
};

export default NumericalGraph;
