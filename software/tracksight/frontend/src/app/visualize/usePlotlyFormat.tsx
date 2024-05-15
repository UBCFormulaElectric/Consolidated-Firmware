'use client';
import { Dispatch, SetStateAction, useState, useEffect } from "react";
import { getRandomColor } from "./graph";

export function usePlotlyFormat(setGraphTitle: (title: string) => void): [Plotly.Data[], Dispatch<SetStateAction<Record<string, { times: Array<string>; values: Array<number>; }>>>] {
    const [data, setData] = useState<Record<string, { times: Array<string>; values: Array<number>; }>>({});
    const [formattedData, setFormattedData] = useState<Plotly.Data[]>([]);
    useEffect(() => {
        setGraphTitle(Object.keys(data).join(" + "));
        setFormattedData(Object.entries(data).map(([graphName, { times, values }]) => ({
            name: graphName,
            x: times, y: values,
            type: 'scatter', mode: 'lines+markers', line: { color: getRandomColor() }
        } as Plotly.Data)));
    }, [data]);

    return [formattedData, setData];
}
