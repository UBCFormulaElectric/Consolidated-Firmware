'use client';

import { Dispatch, useEffect, useState } from 'react';
import { Button, Space } from 'antd';

import DropdownMenu from './dropdown_menu';
import TimeStampPicker from './timestamp_picker';
import { MessageInstance } from 'antd/es/message/interface';

export interface QueryDataProps {
   url: string,
   setData: Dispatch<{[name: string]: {time: Array<string>, value: Array<number>}}>,
   messageApi: MessageInstance, 
}

const QueryData = (props: QueryDataProps) => {
    const [measurement, setMeasurement] = useState<string[]>([]);
    const [allMeasurements, setAllMeasurements] = useState<string[]>([]);

    const [fields, setFields] = useState<string[]>([]);
    const [allFields, setAllFields] = useState<string[]>([]);

    const [startEpoch, setStartEpoch] = useState<string>("");
    const [endEpoch, setEndEpoch] = useState<string>("");

    useEffect(() => {
        fetch(props.url + "/signal/measurement", {
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
        fetch(props.url + "/signal/fields/" + measurement_name, {
            method: 'get',
        }).then((response) => response.json())
          .then((data) => setAllFields(data))
          .catch((error) => console.log(error));
    }, [measurement]);

    const handleSubmit = () => {
        if (!startEpoch || !endEpoch || !measurement || !fields.length) {
            props.messageApi.open({type: "error", content: "Please fill out all fields properly"});
            return;
        }
        const newParams = new URLSearchParams({measurement: measurement[0],  start_epoch: startEpoch, end_epoch: endEpoch });
        for (const field in fields) {
            newParams.append('fields', fields[field]);
        }
        fetch(props.url + "/query?" + newParams)
        .then((response) => {
            if (!response.ok) {
                return response.json().then(json => {throw new Error(json["error"])});
            } else {
                return response.json()
            }
        })
        .then((data) => props.setData(data))
        .catch((error) => props.messageApi.open({type: "error", content: error.toString()}));

    };

    return (
        <div style={{display: 'flex', flexDirection: 'column'}}>
            <DropdownMenu setOption={setMeasurement} selectedOptions={measurement} options={allMeasurements} single={true} name={"Measurements"}/>
            <DropdownMenu setOption={setFields} selectedOptions={fields} options={allFields} single={false} name={"Fields"}/>
            <TimeStampPicker setStart={setStartEpoch} setEnd={setEndEpoch} />
            <Button onClick={handleSubmit}>submit</Button>
        </div>);
}

export default QueryData;
