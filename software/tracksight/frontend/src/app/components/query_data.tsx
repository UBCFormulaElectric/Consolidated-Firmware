'use client';

import { useEffect, useState } from 'react';
import { Button, Space } from 'antd';

import DropdownMenu from './dropdown_menu.tsx';

const QueryData = (props) => {
    const [signals, setSignals] = useState([]);
    const [avail, setAvail] = useState([]);
    const [temp_data, set_temp_data] = useState({});

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
