'use client';

import { useEffect, useState } from 'react';
import { Button, Space } from 'antd';

import DropdownMenu from './dropdown_menu';
import TimeStampPicker from './timestamp_picker';

// pass in bool to determine how to query (live or not)
// add a guard for each (i.e if true then query live, else query http)
// props = url(for http), socket(for socket), live(bool for live or http),
// setData(for setting a graph's data)
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

        if (!startEpoch || !endEpoch || !measurement || !fields) {
            console.log("TODO: Implement proper error handling");
            return;
        }
        fetch(props.url + "/query?" + new URLSearchParams({
            'measurement': measurement[0],
            'fields': fields,
            'start_epoch': startEpoch,
            'end_epoch': endEpoch,
        })).then((response) => response.json())
           .then((data) => props.setData(data))
           .catch((error) => console.log(error));
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
