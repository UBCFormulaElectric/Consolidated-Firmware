'use client';
import { Dispatch, useEffect, useState } from 'react';
import { Space, Switch } from 'antd';

import LiveDropdownMenu from './dropdown_live';
import { MessageInstance } from 'antd/es/message/interface';

const MAX_DATA_POINTS = 100; // max amount of data points on the graph at a time
const UPDATE_INTERVAL_MS = 1000; // how often the graph updates

export interface QueryLiveProps {
    url: string,
    setData: Dispatch<{ [name: string]: { times: Array<string>, values: Array<number> } }>,
    messageApi: MessageInstance,
}

const QueryLive = (props: QueryLiveProps) => {
    const measurement = "live";

    const [signals, setSignals] = useState<string[]>([]);
    const [allSignal, setAllSignal] = useState<string[]>([]);
    const [useLive, setUseLive] = useState<boolean>(false);
    const [avail, setAvail] = useState<string[]>([]);

    const changeLive = (checked: boolean) => {
        setUseLive(checked);
    }

    useEffect(() => {
        fetch(props.url + "/signals/" + measurement, {
            method: 'get',
        }).then((response) => response.json())
            .then((data) => setAllSignal(data))
            .catch((error) => console.log(error));
    }, [useLive]);

    return (
        <div style={{ display: 'flex', flexDirection: 'column' }}>
            <Space direction={"vertical"} size={"small"}>
                <p>Turn live signal on/off</p>
                <Switch onChange={changeLive} checked={useLive} />
                <Space size={"middle"}>
                    <LiveDropdownMenu setSignal={setSignals} signals={signals} avail={avail} />
                </Space>
            </Space>
        </div>);
}

export default QueryLive;