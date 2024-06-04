/**
 * REST API Endpoints:
 * /data/measurement 
 * /data/fields/<measurement>
 * /query
 */

'use client';
// ui
import { Button, DatePicker } from 'antd';
import TimePlot from './timeplot';
import { toast } from "sonner"
// constants
import { FLASK_URL } from '@/app/constants';
// types
import { GraphI } from '@/types/Graph';
import { Nullable } from '@/types/Utility';
import { PlotRelayoutEvent } from 'plotly.js';
import { Dispatch, MouseEventHandler, SetStateAction, useEffect, useState } from 'react';
import { Dayjs } from 'dayjs';
import { MeasurementDropdown, FieldDropdown, usePlotlyFormat } from './graph_components'


export default function Graph({ syncZoom, sharedZoomData, setSharedZoomData, deletePlot }: {
    graphInfo: GraphI,
    deletePlot: MouseEventHandler<HTMLButtonElement>,
    syncZoom: boolean,
    sharedZoomData: PlotRelayoutEvent,
    setSharedZoomData: Dispatch<SetStateAction<PlotRelayoutEvent>>
}) {
    const [plotTitle, setPlotTitle] = useState<string>("");
    const [plotData, setPlotData] = usePlotlyFormat(setPlotTitle);

    // Top Form Information
    const [measurement, setMeasurement] = useState<string | null>(null);
    const [fields, setFields] = useState<string[]>([]);
    const [startEpoch, setStartEpoch] = useState<number | null>(null);
    const [endEpoch, setEndEpoch] = useState<number | null>(null);

    return (
        <TimePlot plotTitle={plotTitle} deletePlot={deletePlot}
            plotData={plotData} clearPlotData={e => setPlotData({})}
            syncZoom={syncZoom} sharedZoomData={sharedZoomData} setSharedZoomData={setSharedZoomData}
        >
            <div className="flex flex-col gap-y-2">
                <MeasurementDropdown measurement={measurement} setMeasurement={setMeasurement} />
                <FieldDropdown fields={fields} setFields={setFields} measurement={measurement} />
                <DatePicker.RangePicker showTime
                    onChange={(date_pair: Nullable<[Nullable<Dayjs>, Nullable<Dayjs>]>, _formatted_dates: [string, string]) => {
                        if (!date_pair) return;
                        if (!date_pair[0] || !date_pair[1]) return console.warn("Partially null date pair");
                        setStartEpoch(date_pair[0].unix());
                        setEndEpoch(date_pair[1].unix());
                    }}
                />
                <Button onClick={async (e) => {
                    const missingQueryEls = !startEpoch || !endEpoch || !measurement || fields.length == 0;
                    if (missingQueryEls) {
                        toast("Please fill out all fields properly")
                        return;
                    }
                    const fetchUrl = new URL("/data/query", FLASK_URL);
                    fetchUrl.search = new URLSearchParams({
                        measurement: measurement,
                        start_epoch: startEpoch.toString(), end_epoch: endEpoch.toString(), // apparently for some reason the time is given in ms
                        signals: fields.join(",")
                    }).toString();
                    try {
                        const res = await fetch(fetchUrl, { method: 'get' })
                        if (!res.ok) {
                            console.error(await res.json())
                            toast.error("Error fetching data")
                            return;
                        }
                        setPlotData(await res.json())
                    } catch (error) {
                        console.error(error)
                    }
                }}>
                    Submit
                </Button>
            </div>
        </TimePlot>
    );
}