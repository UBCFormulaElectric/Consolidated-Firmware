'use client';
import { Dispatch, useEffect, useState } from 'react';
import { Button } from 'antd';

import DropdownMenu from './dropdown_menu';
import TimeStampPicker from './timestamp_picker';

const QueryData = (props: {
    url: string,
    setData: Dispatch<{ [name: string]: { times: Array<string>, values: Array<number> } }>,
}) => {
    const [measurement, setMeasurement] = useState<string[]>([]);
    const [allMeasurements, setAllMeasurements] = useState<string[]>([]);

    const [signals, setSignals] = useState<string[]>([]);
    const [allSignal, setAllSignal] = useState<string[]>([]);

    const [startEpoch, setStartEpoch] = useState<number | null>(null);
    const [endEpoch, setEndEpoch] = useState<number | null>(null);

    useEffect(() => {
        fetch(props.url + "/measurements", {
            method: 'get',
        }).then((response) => response.json())
            .then((data) => setAllMeasurements(data))
            .catch((error) => console.log(error));
    }, []);

    useEffect(() => {
        if (!measurement.length) {
            return;
        }

        const measurement_name = measurement[0];
        fetch(props.url + "/signals/" + measurement_name, {
            method: 'get',
        }).then((response) => response.json())
            .then((data) => setAllSignal(data))
            .catch((error) => console.log(error));
    }, [measurement]);

    const handleSubmit = () => {
        if (!startEpoch || !endEpoch || !measurement || !signals.length) {
            // TODO replace with shadcn sonner
            // props.messageApi.open({ type: "error", content: "Please fill out all signal properly" });
            return;
        }
        const newParams = new URLSearchParams({
            measurement: measurement[0], start_epoch: startEpoch.toString(), end_epoch: endEpoch.toString(),
            signals: signals.join(","),
        });
        fetch(props.url + "/query?" + newParams)
            .then((response) => {
                if (!response.ok) {
                    return response.json().then(json => { throw new Error(json["error"]) });
                } else {
                    return response.json()
                }
            })
            .then((data) => props.setData(data))
            // TDOO replace with shadcn sonner
            // .catch((error) => props.messageApi.open({ type: "error", content: error.toString() }));

    };

    return (
        <div style={{ display: 'flex', flexDirection: 'column' }}>
            <DropdownMenu setOption={setMeasurement} selectedOptions={measurement} options={allMeasurements} single={true} name={"Measurement"} />
            <DropdownMenu setOption={setSignals} selectedOptions={signals} options={allSignal} single={false} name={"Signals"} />
            <TimeStampPicker setStart={setStartEpoch} setEnd={setEndEpoch} />
            <Button onClick={handleSubmit}>Submit</Button>
        </div>);
}

export default QueryData;