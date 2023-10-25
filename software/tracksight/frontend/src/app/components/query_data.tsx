'use client';

import { useEffect, useState } from 'react';
import { Button, Space } from 'antd';

import DropdownMenu from './dropdown_menu.tsx';

// pass in bool to determine how to query (live or not)
// add a guard for each (i.e if true then query live, else query http)

// props = url(for http), socket(for socket), live(bool for live or http),
// setData(for setting a graph's data)
const QueryData = (props) => {
    const [signals, setSignals] = useState([]);
    const [avail, setAvail] = useState([]);

    useEffect(() => {
        fetch(props.url + "/signal", {
            method: 'GET',
        }).then((response) => response.json())
          .then((data) => setAvail(data))
          .catch((error) => console.log(error));
    }, []);

    const handleSubmit = () => {
        const promises = [];
        const temp_data = {};
        for (const signal of signals) {
            promises.push(
                fetch(props.url + "/signal/" + signal, {
                    method: 'GET',
                }).then((response) => response.json())
                  .then((data) => {
                      temp_data[signal] = data;
                  })
                  .catch((error) => console.log(error))
            );
        }
        Promise.all(promises).then(e => props.setData(temp_data));
    };

    return (
        <Space>
            <DropdownMenu url={props.url} setSignal={setSignals} signals={signals} avail={avail} />
            <Button onClick={handleSubmit}>submit</Button>
        </Space>);
}

export default QueryData;
