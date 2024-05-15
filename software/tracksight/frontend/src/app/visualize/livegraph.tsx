'use client'
import { useState, useEffect, MouseEventHandler, Dispatch, SetStateAction } from 'react';
import Plot from 'react-plotly.js';
import { Button, Card, Switch, Space } from 'antd';
import { useSocket } from '@/app/useSocket';
import { FLASK_URL } from '@/app/constants';
import DropdownMenu from './dropdown_menu';
import { assertType } from '@/types/Assert';
import { PlotData } from 'plotly.js';

interface FormattedData {
    x: Date[];
    y: number[];
    type: string;
    mode: string;
    name: string;
}

const EMPTY_FORMATTED_DATA: FormattedData = {
    x: [],
    y: [],
    type: 'scatter',
    mode: 'lines+markers',
    name: 'test',
};

const DEFAULT_LAYOUT: Partial<Plotly.Layout> = {
    width: 620,
    height: 500,
    title: "Live Data",
    xaxis: {
        // for formatting time
        tickformat: "%H:%M:%S.%L", // TODO fix this formatting
        automargin: true,
    },
    yaxis: {},
    legend: { "orientation": "h" },
};

/**
 * Websockets that are used (handling signals, and handling available signals)
 *  - signal_sub (emit), signal_unsub (emit)
 *  - signal_response
 *  - signal_stopped
 *  - available_signals_sub (emit)
 *  - available_signals_response
 */
export default function LiveGraph(props: {
    id: number,
    onDelete: MouseEventHandler<HTMLElement>,
}) {
    const { socket, loading } = useSocket(FLASK_URL);
    // Plot Data 
    const [plotData, setPlotData] = useState<FormattedData[]>([]);
    const [graphLayout, setGraphLayout] = useState<Partial<Plotly.Layout>>(DEFAULT_LAYOUT);
    const setGraphTitle = (t: string) => setGraphLayout(p => ({ ...p, title: t, }))

    // handles live data
    const [isSubscribed, setIsSubscribed] = useState<boolean>(true);
    useEffect(() => {
        if (!socket) return;
        if(!isSubscribed) return;
        socket.on("signal_response", (packet: unknown) => {
            if (typeof packet !== 'object' || packet === null) {
                throw new Error("Invalid packet received from server: is of wrong type or null");
            }
            if (!('id' in packet && typeof packet.id === "string" && 'signal' in packet && typeof packet.signal === "object" && packet.signal != null)) {
                return; // TODO display error
            }
            console.log(packet.id);
            const signals = packet.signal;
            assertType<object>(signals, typeof signals === 'object', "Recieved signal is not an object");

            setPlotData(p => {
                // extracts the most recent 100 data points to display to ensure the graph doesn't get too cluttered
                // const sortedDates = Object.keys(signalData).sort((a, b) => new Date(a).getTime() - new Date(b).getTime());
                // const maxDataPoints = sortedDates.slice(-MAX_DATA_POINTS);
                const existingSignal = p.find((d) => d.name === packet.id) || EMPTY_FORMATTED_DATA;
                // TODO new data!!!
                existingSignal.x.push(new Date());
                existingSignal.y.push(Math.random());
                return [...p.filter(d => d.name != packet.id), existingSignal];
            });
        })
        socket.on("signal_stopped", (data) => {
            // TODO display that a signal has stopped broadcasting
            console.log(`Signal ${data.id} stopped`)
        })

        return () => {
            socket.off("signal_response");
        };
    }, [socket, isSubscribed])

    // Handles which signals we care about
    const [prevWatchSignals, setPrevWatchSignals] = useState<string[]>([]); // technically an anti-pattern but the handler is inside an opaque component
    const [watchSignals, setWatchSignals] = useState<string[]>([]);
    useEffect(() => {
        if (watchSignals.length === 0) return;
        if (!socket || loading) throw new Error("Socket not initialized");
        watchSignals.forEach(s => {
            if (!(s in prevWatchSignals)) socket.emit("signal_sub", { signal: s });
        })
        prevWatchSignals.forEach(s => {
            if (!(s in watchSignals)) socket.emit("signal_unsub", { signal: s });
        });
        // cleanup
        setGraphTitle(Object.keys(watchSignals).join(" + "));
        return () => {
            setPrevWatchSignals(watchSignals);
        }
    }, [watchSignals]);

    // Handles all available signals
    const [validSignals, setValidSignals] = useState<string[]>([]);
    useEffect(() => {
        if(!socket) return;
        socket.emit("available_signals_sub");
        socket.on("available_signals_response", (signalNames) => {
            setValidSignals(signalNames);
        });
        return () => {
            socket.off("available_signals_response");
        };
    }, [socket]);

    return (
        <Card bodyStyle={{ display: 'flex', flexDirection: 'column' }}>
            <div className="flex flex-row items-center gap-x-2 mb-2">
                <p>Live</p>
                <Switch onChange={setIsSubscribed} checked={isSubscribed} />
            </div>
            <div className="flex flex-row gap-x-2">
                <DropdownMenu options={validSignals} setSelectedOptions={setWatchSignals}
                    loading={!loading} disabled={!loading}
                    placeholder="Signals"
                />
            </div>
            <Plot data={plotData as Partial<PlotData>[]} layout={graphLayout} />
            <Space.Compact size={"middle"}>
                <Button block={true} className="clear" onClick={() => {
                    setGraphLayout(DEFAULT_LAYOUT);
                    setPlotData([]);
                }}>
                    Clear
                </Button>
                <Button block={true} danger={true} ghost={false} onClick={props.onDelete}>Delete This Graph</Button>
            </Space.Compact>
        </Card>
    );
}


// for live data simulation, remove when live signals are actually implemented
// for testing purposes only. uncomment to see live data simulation
/*
useEffect(() => {
    if (useLive) return;
    const interval = setInterval(() => {
        const time = new Date().toISOString();
        const value = Math.random() * 0.5;

        // Update the data state with the new point
        setData((prevData) => {
            let updatedData = { ...prevData };
            updatedData = { ...updatedData['signals'] };
            // Update the signal you want to mimic live data for
            updatedData['LiveTest'] = {
                ...updatedData['LiveTest'],
                [time]: value,
            };
            const ret = {'id': props.id, 'signals': updatedData};
            console.log(ret);
            return ret;
        });
    }, UPDATE_INTERVAL_MS);

    return () => {
        clearInterval(interval);
    };
}, [useLive]);
*/