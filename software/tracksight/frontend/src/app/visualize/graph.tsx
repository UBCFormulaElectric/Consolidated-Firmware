/**
 * REST API Endpoints:
 * /signal/measurement 
 * /signal/fields/<measurement>
 * /query
 */

'use client';
import { Dispatch, MouseEventHandler, SetStateAction, useEffect, useState } from 'react';
import { PlotData, PlotRelayoutEvent } from 'plotly.js';
import { Button } from 'antd';
import { GraphI } from '@/types/Graph';
import DropdownMenu from './dropdown_menu';
import TimeStampPicker from './timestamp_picker';
import { FLASK_URL } from '@/app/constants';
import TimePlot, { FormattedData } from './timeplot';

export const getRandomColor = () => {
    const r = Math.floor(Math.random() * 256);
    const g = Math.floor(Math.random() * 256);
    const b = Math.floor(Math.random() * 256);
    return `rgb(${r},${g},${b})`;
};

const MeasurementDropdown = ({ measurement, setMeasurement }: {
    measurement: string,
    setMeasurement: Dispatch<SetStateAction<string>>
}) => {
    const [allMeasurements, setAllMeasurements] = useState<string[]>([]);
    const [loading, setLoading] = useState<boolean>(true);
    useEffect(() => {
        (async () => {
            const fetchURL = `${FLASK_URL}/signal/measurements`
            const res = await fetch(fetchURL, {
                method: 'get',
            })
            if (!res.ok) {
                console.error(await res.text())
                // TODO messages fetch error
                return;
            }
            const data = await res.json()
            setAllMeasurements(data)
            setLoading(false)
        })()
    }, []);

    return (
        <DropdownMenu
            selectedOptions={measurement}
            setSelectedOptions={setMeasurement}
            options={allMeasurements}
            single={true}
            placeholder={"Measurements"}
            disabled={loading}
            loading={loading}
        />
    )
}

const FieldDropdown = ({ fields, setFields, measurement }: {
    fields: string[],
    setFields: Dispatch<SetStateAction<string[]>>
    measurement: string,
}) => {
    const [allFields, setAllFields] = useState<string[]>([]);
    const [hasFetchedFields, setHasFetchedFields] = useState<boolean>(false);
    const [loading, setLoading] = useState<boolean>(false);

    useEffect(() => {
        setFields([]);
        if (measurement.length == 0) return;
        (async () => {
            setLoading(true);
            setHasFetchedFields(false)
            try {
                const res = await fetch(new URL(`/signal/measurement/${measurement}/fields`, FLASK_URL), {
                    method: 'get',
                })
                if (!res.ok) {
                    console.error(await res.text())
                    return;
                }
                setAllFields(await res.json())
                setHasFetchedFields(true)
            } catch (error) {
                console.error(error)
            } finally {
                setLoading(false)
            }
        })()
    }, [measurement]);

    return (
        <DropdownMenu
            disabled={!hasFetchedFields}
            loading={loading}
            selectedOptions={fields}
            setSelectedOptions={setFields}
            options={allFields}
            single={false}
            placeholder="Fields"
        />
    )
}

function usePlotlyFormat(setGraphTitle: (title: string) => void): [
    FormattedData[],
    Dispatch<SetStateAction<Record<string, { times: Array<string>; values: Array<number>; }>>>
] {
    const [data, setData] = useState<Record<string, { times: Array<string>; values: Array<number>; }>>({});
    const [formattedData, setFormattedData] = useState<FormattedData[]>([]);
    useEffect(() => {
        setGraphTitle(Object.keys(data).join(" + "));
        setFormattedData(Object.entries(data).map(([graphName, { times, values }]) => ({
            name: graphName,
            x: times.map(x=>new Date(x)), y: values,
            type: "scatter", mode: "lines+markers", line: { color: getRandomColor() }
        })));
    }, [data]);

    return [formattedData, setData];
}

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
    const [measurement, setMeasurement] = useState<string>("");
    const [fields, setFields] = useState<string[]>([]);
    const [startEpoch, setStartEpoch] = useState<string>("");
    const [endEpoch, setEndEpoch] = useState<string>("");

    return (
        <TimePlot plotTitle={plotTitle} deletePlot={deletePlot}
            plotData={plotData} clearPlotData={e => setPlotData({})}
            syncZoom={syncZoom} sharedZoomData={sharedZoomData} setSharedZoomData={setSharedZoomData}
        >
            <MeasurementDropdown measurement={measurement} setMeasurement={setMeasurement} />
            <FieldDropdown fields={fields} setFields={setFields} measurement={measurement} />
            <TimeStampPicker setStart={setStartEpoch} setEnd={setEndEpoch} />
            <Button onClick={async (e) => {
                const missingQueryEls = !startEpoch || !endEpoch || !measurement || fields.length == 0;
                if (missingQueryEls) {
                    // TODO add message
                    // "Please fill out all fields properly"
                    return;
                }
                const fetchUrl = new URL("/signal/query", FLASK_URL);
                fetchUrl.search = new URLSearchParams({
                    measurement: measurement,
                    start_epoch: startEpoch.slice(0, -2 - 3), end_epoch: endEpoch.slice(0, -2 - 3), // apparently for some reason the time is given in ms
                    fields: fields.join(",")
                }).toString();
                console.log(fetchUrl.toString()) // TODO remove after testing

                try {
                    const res = await fetch(fetchUrl, { method: 'get' })
                    if (!res.ok) {
                        // TODO add message
                        console.error(await res.json())
                        return;
                    }
                    setPlotData(await res.json())
                } catch (error) {
                    console.error(error)
                }
            }}>
                Submit
            </Button>
        </TimePlot>
    );
}