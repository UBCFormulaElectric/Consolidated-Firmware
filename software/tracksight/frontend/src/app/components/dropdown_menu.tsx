import { useState, useEffect, React } from 'react';
import { DownOutlined, SmileOutlined } from '@ant-design/icons';
import type { MenuProps } from 'antd';
import { Dropdown, Space } from 'antd';
import WebSocketComponent from './web_socket';


const DropdownMenu = (props) => {
    const [data, setData] = useState([]); // Format is list of signals
    const [items, setItems] = useState([]);
    const [selectedSignal, setSelectedSignal] = useState("");

    // Request available signals through socket on render
    useEffect(() => {
        props.socket.emit("available_signals", {"ids": []});
        
        // Listen for the server's response
        props.socket.on("available_signals_response", (signalNames) => {
            // Update the state with new data
            setData(signalNames);
        });

        // Cleanup
        return () => {
            props.socket.off("available_signals_response");
        };
    }, [props.socket]);

    // Update dropdown items whenever data changes
    useEffect(() => {
        const updatedItems = data.map((signalName, index) => ({
            key: index.toString(),
            label: (
                <p onClick={() => setSelectedSignal(signalName)}>
                  {signalName}
                </p>
              ),
        }));
        setItems(updatedItems);
    }, [data]);

    return (
        <div>
            <WebSocketComponent socket={props.socket} setData={props.setData} selectedSignal={selectedSignal} />
            <Dropdown menu={{ items }}>
                <a onClick={(e) => e.preventDefault()}>
                    <Space>
                        Signals
                        <DownOutlined />
                    </Space>
                </a>
            </Dropdown>
        </div>
    );
};

export default DropdownMenu;
