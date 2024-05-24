/**
 * REST API Endpoints:
 * /signal/measurement 
 * /signal/fields/<measurement>
 * /query
 */

'use client';
import { Dispatch, MouseEventHandler, SetStateAction, useEffect, useState } from 'react';
import { PlotRelayoutEvent } from 'plotly.js';
import { Button, DatePicker } from 'antd';
import { GraphI } from '@/types/Graph';
import DropdownMenu from './dropdown_menu';
import { FLASK_URL } from '@/app/constants';
import TimePlot, { FormattedData } from './timeplot';
import { Dayjs } from 'dayjs';
import { Nullable } from '@/types/Util';

export const getRandomColor = () => {
    const r = Math.floor(Math.random() * 256);
    const g = Math.floor(Math.random() * 256);
    const b = Math.floor(Math.random() * 256);
    return `rgb(${r},${g},${b})`;
};

function MeasurementDropdown({ measurement, setMeasurement }: {
    measurement: string | null;
    setMeasurement: Dispatch<SetStateAction<string | null>>;
}) {
    const [allMeasurements, setAllMeasurements] = useState<string[]>([]);
    const [loading, setLoading] = useState<boolean>(true);
    useEffect(() => {
        (async () => {
            const fetchURL = `${FLASK_URL}/signal/measurements`;
            const res = await fetch(fetchURL, {
                method: 'get',
            });
            if (!res.ok) {
                console.error(await res.text());
                // TODO messages fetch error
                return;
            }
            const data = await res.json();
            setAllMeasurements(data);
            setLoading(false);
        })();
    }, []);

    return (
        <DropdownMenu
            selectedOptions={measurement}
            setSelectedOptions={setMeasurement}
            options={allMeasurements}
            single={true}
            placeholder={"Measurements"}
            disabled={loading}
            loading={loading} />
    );
}

function FieldDropdown({ fields, setFields, measurement }: {
    fields: string[];
    setFields: Dispatch<SetStateAction<string[]>>;
    measurement: string | null;
}) {
    const [allFields, setAllFields] = useState<string[]>([]);
    const [hasFetchedFields, setHasFetchedFields] = useState<boolean>(false);
    const [loading, setLoading] = useState<boolean>(false);

    useEffect(() => {
        setFields([]);
        if (measurement === null) return;
        (async () => {
            setLoading(true);
            setHasFetchedFields(false);
            try {
                const res = await fetch(new URL(`/signal/measurement/${measurement}/fields`, FLASK_URL), {
                    method: 'get',
                });
                if (!res.ok) {
                    console.error(await res.text());
                    return;
                }
                setAllFields(await res.json());
                setHasFetchedFields(true);
            } catch (error) {
                console.error(error);
            } finally {
                setLoading(false);
            }
        })();
    }, [measurement]);

    return (
        <DropdownMenu
            disabled={!hasFetchedFields}
            loading={loading}
            selectedOptions={fields}
            setSelectedOptions={setFields}
            options={allFields}
            single={false}
            placeholder="Fields" />
    );
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
            x: times.map(x => new Date(x)), y: values,
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
                        if(!date_pair[0] || !date_pair[1]) return console.warn("Partially null date pair");
                        setStartEpoch(date_pair[0].unix());
                        setEndEpoch(date_pair[1].unix());
                    }}
                />
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
                        start_epoch: startEpoch.toString(), end_epoch: endEpoch.toString(), // apparently for some reason the time is given in ms
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
            </div>
        </TimePlot>
    );
}