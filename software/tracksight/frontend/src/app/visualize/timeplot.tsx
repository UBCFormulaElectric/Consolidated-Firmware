"use client";
import dynamic from "next/dynamic";
import { PlotData, PlotRelayoutEvent } from "plotly.js";
import { Dispatch, MouseEventHandler, ReactNode, SetStateAction, useEffect, useState } from "react";

const Plot = dynamic(() => import("react-plotly.js"), { ssr: false, })

export interface FormattedData extends Pick<PlotData, "type" | "mode" | "name"> {
    x: Date[],
    y: number[],
    line?: PlotData["line"],
};

const DEFAULT_LAYOUT: Partial<Plotly.Layout> = {
    width: 620,
    height: 500,
    title: "Empty",
    xaxis: {},
    yaxis: {},
    legend: { "orientation": "h" },
}

// TODO refactor shared zoom data to be a context
export default function TimePlot({ children, plotTitle, syncZoom, sharedZoomData, setSharedZoomData, plotData, clearPlotData, deletePlot }: {
    children: ReactNode,
    plotTitle: string,
    plotData: FormattedData[],
    clearPlotData: MouseEventHandler<HTMLButtonElement>,
    syncZoom: boolean,
    sharedZoomData: PlotRelayoutEvent,
    setSharedZoomData: Dispatch<SetStateAction<PlotRelayoutEvent>>,
    deletePlot: MouseEventHandler<HTMLButtonElement>,
}) {
    //default graph layout
    const [graphLayout, setGraphLayout] = useState<Partial<Plotly.Layout>>(DEFAULT_LAYOUT);
    // updates graph layout when zoomed 
    useEffect(() => {
        if (!(sharedZoomData && 'xaxis.range[0]' in sharedZoomData)) {
            // TODO error in some way
            return;
        }
        // Update the graph's layout with the new axis ranges
        setGraphLayout(prevLayout => ({
            ...prevLayout,
            xaxis: {
                range: [sharedZoomData['xaxis.range[0]'], sharedZoomData['xaxis.range[1]']],
            },
            yaxis: {
                range: [sharedZoomData['yaxis.range[0]'], sharedZoomData['yaxis.range[1]']],
            },
        }));
    }, [sharedZoomData]);

    useEffect(() => {
        setGraphLayout(l => ({ ...l, title: plotTitle }))
    }, [plotTitle])

    return (
        <div className="flex flex-col p-4 border-[1.5px] rounded-xl">
            {/* Measurement Selector */}
            <div className="flex-1">
                {children}
            </div>

            {/* TODO better plotting library :(((( */}
            <Plot layout={graphLayout} data={plotData} // Pass the array of formatted data objects
                config={{ displayModeBar: true, displaylogo: false, scrollZoom: true, }}
                onRelayout={(e: PlotRelayoutEvent) => { if (syncZoom) setSharedZoomData(e) }}
            />

            <div className="flex flex-row gap-x-2">
                <button className="bg-[#1890ff] hover:bg-blue-400 text-white text-sm
                    transition-colors duration-100 border-0 block p-2 rounded-md flex-1"
                    onClick={e => {
                        setGraphLayout(DEFAULT_LAYOUT);
                        clearPlotData(e);
                    }}
                >
                    Clear Data
                </button>
                <button className="bg-[#ff4d4f] hover:bg-red-400 text-white text-sm
                    transition-colors duration-100 border-0 block p-2 rounded-md flex-1"
                    onClick={deletePlot}
                >
                    Delete This Graph
                </button>
            </div>
        </div>
    )
}