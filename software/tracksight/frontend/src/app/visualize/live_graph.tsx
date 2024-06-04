/**
 * REST API Endpoints:
 * /data/measurement 
 * /data/fields/<measurement>
 * /query
 */

'use client';
// ui
import { Switch } from 'antd';
import TimePlot from './timeplot';
import { toast } from "sonner"
// constants
import { FLASK_URL } from '@/app/constants';
// types
import { GraphI } from '@/types/Graph';
import { PlotRelayoutEvent } from 'plotly.js';
import { Dispatch, MouseEventHandler, SetStateAction, useEffect, useState } from 'react';
import { FieldDropdown, usePlotlyFormat } from './graph_components'

export default function LiveGraph({ syncZoom, sharedZoomData, setSharedZoomData, deletePlot }: {
    graphInfo: GraphI,
    deletePlot: MouseEventHandler<HTMLButtonElement>,
    syncZoom: boolean,
    sharedZoomData: PlotRelayoutEvent,
    setSharedZoomData: Dispatch<SetStateAction<PlotRelayoutEvent>>
}) {
    const [plotTitle, setPlotTitle] = useState<string>("");
    const [plotData, setPlotData] = usePlotlyFormat(setPlotTitle);

    // Top Form Information
    const [fields, setFields] = useState<string[]>([]);
    const [isSubscribed, setIsSubscribed] = useState<boolean>(true);
    const live_measurement = "live"

    const fetchLiveData = async () => {
        const missingQueryEls = fields.length === 0;
        if (missingQueryEls) {
            toast("Please fill out all fields properly");
            return;
        }
        const fetchUrl = new URL("/data/query/live", FLASK_URL);
        fetchUrl.search = new URLSearchParams({
            signals: fields.join(",")
        }).toString();

        try {
            const res = await fetch(fetchUrl);
            if (!res.ok) {
                console.error(await res.json());
                toast.error("Error fetching data");
                return;
            }
            setPlotData(await res.json());
        } catch (error) {
            console.error(error);
            toast.error("Network error");
        }
    };

    useEffect(() => {
        let interval = null;
        if (isSubscribed && fields.length > 0) {
            interval = setInterval(fetchLiveData, 1000); // Run at 1Hz
        } else {
            if (interval) clearInterval(interval);
        }
        return () => {
            if (interval) clearInterval(interval);
        };
    }, [isSubscribed, fields]);

    return (
        <TimePlot plotTitle={plotTitle} deletePlot={deletePlot}
            plotData={plotData} clearPlotData={e => setPlotData({})}
            syncZoom={syncZoom} sharedZoomData={sharedZoomData} setSharedZoomData={setSharedZoomData}
        >
            <div className="flex flex-col gap-y-2">
                <FieldDropdown fields={fields} setFields={setFields} measurement={live_measurement} />
                <div className="flex flex-row items-center gap-x-2 mb-2">
                    <p>Live</p>
                    <Switch onChange={setIsSubscribed} checked={isSubscribed} />
                </div>
            </div>
        </TimePlot>
    );
}