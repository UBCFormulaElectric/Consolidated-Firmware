import { useState, useEffect, React } from 'react';
import { DownOutlined, SmileOutlined } from '@ant-design/icons';
import type { MenuProps } from 'antd';
import { Dropdown, Space } from 'antd';
import WebSocketComponent from './web_socket';


const DropdownMenu = (props) => {
    const [data, setData] = useState(["Signal1", "Signal2"]) // Format is list of signals 
    const [items, setItems] = useState<MenuProps['items']>([]);
    const noItems: [] = [{
        key: "1",
        label: "No Avaliable Signals"
    }];

    // Request available signals through socket on render
    props.socket.emit("available_signals", {"ids" : []});

    useEffect(() => {
        if (data.length > 0) {
            const res = []
            for (let i = 0; i < data.length; i++) {
                res.push({
                    key: i.toString(),
                    label: (
                        data[i]
                    )
                })
            }
            setItems(res)
        } else {
            setItems(noItems)
        }


        return () => {
            props.socket.off("available_signals");
        }
   
    
    }, [data]);

    props.socket.getAvailableSignals

    return (
        <div>
            <WebSocketComponent socket={props.socket} setAvailableSignals={setData} />
            <Dropdown menu={{ items }}>
                <a onClick={(e) => e.preventDefault()}>
                    <Space>
                        Signals
                        <DownOutlined />
                    </Space>
                </a>
            </Dropdown>
        </div>);
};

export default DropdownMenu;
