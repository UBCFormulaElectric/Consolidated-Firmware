'use client';

import { useEffect, useState } from 'react';
import { Button, Space } from 'antd';

import DropdownMenu from './dropdown_menu.tsx';
import TimeStampPicker from './timestamp_picker.tsx';

const QueryData = (props) => {
    const [measurement, setMeasurement] = useState([]);
    const [allMeasurements, setAllMeasurements] = useState([]);

    const [fields, setFields] = useState([]);
    const [allFields, setAllFields] = useState([]);

    const [startEpoch, setStartEpoch] = useState(null);
    const [endEpoch, setEndEpoch] = useState(null);

    useEffect(() => {
        fetch(props.url + "/signal", {
            method: 'GET',
        }).then((response) => response.json())
          .then((data) => setAvail(data))
          .catch((error) => console.log(error));

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
        fetch(props.url + "/query?" + new URLSearchParams({
            'measurement': measurement[0],
            'fields': fields,
            'start_epoch': startEpoch,
            'end_epoch': endEpoch,
        })).then((response) => {
                return new Promise((resolve) => response.json()
                                                        .then((json) => resolve({
                                                            status: response.status,
                                                            ok: response.ok,
                                                            json,
                                                        })));
         }).then(({ status, json, ok }) => {
                if (!ok) {
                    props.messageApi.open({type: "error", content: json["error"]})
                } else {
                    props.setData(json);
                }
        }).catch((error) => console.log(error));
    };

    return (
        <div style={{display: 'flex', flexDirection: 'column'}}>
            <DropdownMenu setOption={setMeasurement} selectedOptions={measurement} options={allMeasurements} single={true} name={"Measurements"}/>
            <DropdownMenu setOption={setFields} selectedOptions={fields} options={allFields} name={"Fields"}/>
            <TimeStampPicker setStart={setStartEpoch} setEnd={setEndEpoch} />
            <Button onClick={handleSubmit}>submit</Button>
        </div>);
}

export default QueryData;
