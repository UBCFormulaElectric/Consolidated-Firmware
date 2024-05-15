'use client';
import { useState, useEffect, Dispatch, MouseEventHandler, SetStateAction } from 'react';
import { PlotRelayoutEvent } from 'plotly.js';
import { Button } from 'antd';
import { GraphI } from '@/types/Graph';
import DropdownMenu from './dropdown_menu';
import TimeStampPicker from './timestamp_picker';
import { FLASK_URL } from '@/app/constants';
import Plot from 'react-plotly.js';

const DEFAULT_LAYOUT: Partial<Plotly.Layout> = {
    width: 620,
    height: 500,
    title: "Empty",
    xaxis: {},
    yaxis: {},
    legend: { "orientation": "h" },
}

export const getRandomColor = () => {
    const r = Math.floor(Math.random() * 256);
    const g = Math.floor(Math.random() * 256);
    const b = Math.floor(Math.random() * 256);
    return `rgb(${r},${g},${b})`;
};

const MeasurementDropdown = ({ setMeasurement }: {
    setMeasurement: Dispatch<SetStateAction<string>>
}) => {
    const [allMeasurements, setAllMeasurements] = useState<string[]>([]);
    const [loading, setLoading] = useState<boolean>(true);
    useEffect(() => {
        (async () => {
            const fetchURL = `${FLASK_URL}/signal/measurement`
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
            setSelectedOptions={setMeasurement}
            options={allMeasurements}
            single={true}
            placeholder={"Measurements"}
            disabled={loading}
            loading={loading}
        />
    )
}

const FieldDropdown = ({ setFields, measurement }: {
    setFields: Dispatch<SetStateAction<string[]>>
    measurement: string, 
}) => {
    const [allFields, setAllFields] = useState<string[]>([]);
    const [fetchedFields, setFetchedFields] = useState<boolean>(false);
    const [loading, setLoading] = useState<boolean>(false);

    useEffect(() => {
        if(measurement.length == 0) return;
        (async () => {
            setLoading(true);
            setFetchedFields(false)
            try {
                const res = await fetch(new URL(`/signal/fields/${measurement}`, FLASK_URL), {
                    method: 'get',
                })
                if(!res.ok) {
                    console.error(await res.text())
                    return;
                }
                setAllFields(await res.json())
                setFetchedFields(true)
            } catch (error) {
                console.error(error)
            } finally {
                setLoading(false)
            }
        })()
    }, [measurement]);

    return (
        <DropdownMenu
            disabled={!fetchedFields}
            loading={loading}
            setSelectedOptions={setFields}
            options={allFields}
            single={false}
            placeholder="Fields"
        />
    )
}

function usePlotlyFormat(setGraphTitle: (title: string) => void): [Plotly.Data[], Dispatch<SetStateAction<Record<string, { times: Array<string>; values: Array<number>; }>>>] {
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


export default function Graph({ syncZoom, sharedZoomData, setSharedZoomData, handleDelete }: {
    graphInfo: GraphI,
    syncZoom: boolean,
    sharedZoomData: PlotRelayoutEvent,
    setSharedZoomData: Dispatch<SetStateAction<PlotRelayoutEvent>>
    handleDelete: MouseEventHandler<HTMLInputElement>,
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

    const [plotData, setPlotData] = usePlotlyFormat((t) => setGraphLayout(p => ({ ...p, title: t, })))

    // Top Form Information
    const [measurement, setMeasurement] = useState<string>("");
    const [fields, setFields] = useState<string[]>([]);
    const [startEpoch, setStartEpoch] = useState<string>("");
    const [endEpoch, setEndEpoch] = useState<string>("");

    return (
        <div className="flex flex-col p-4 border-[1.5px] rounded-xl">
            {/* Measurement Selector */}
            <div className="flex flex-col gap-y-2">
                <MeasurementDropdown setMeasurement={setMeasurement} />
                <FieldDropdown setFields={setFields} measurement={measurement} />
                <TimeStampPicker setStart={setStartEpoch} setEnd={setEndEpoch} />
                <Button onClick={async (e) => {
                    const missingQueryEls = !startEpoch || !endEpoch || !measurement || fields.length == 0;
                    if (missingQueryEls) {
                        // TODO add message
                        // "Please fill out all fields properly"
                        return;
                    }
                    const fetchUrl = new URL("/query", FLASK_URL);
                    fetchUrl.search = new URLSearchParams({
                        measurement: measurement[0],
                        start_epoch: startEpoch, end_epoch: endEpoch,
                        fields: fields.join(",")
                    }).toString();
                    console.log(fetchUrl.toString()) // TODO remove after testing

                    try {
                        const res = await fetch(fetchUrl, { method: 'get' })
                        if (!res.ok) {
                            // TODO add message
                            console.error(await res.text())
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

            {/* TODO better plotting library :(((( */}
            <Plot layout={graphLayout} data={plotData} // Pass the array of formatted data objects
                config={{ displayModeBar: true, displaylogo: false, scrollZoom: true, }}
                onRelayout={(e) => { if (syncZoom) setSharedZoomData(e) }}
            />

            <div className="flex flex-row gap-x-2">
                <Button block={true} className="clear" onClick={() => {
                    setGraphLayout(DEFAULT_LAYOUT);
                    setPlotData({});
                }}>
                    Clear Data
                </Button>
                <Button block={true} danger={true} ghost={false} onClick={handleDelete}>
                    Delete This Graph
                </Button>
            </div>
        </div>
    );
}