import { useState, useEffect, React } from 'react';
import { DownOutlined, SmileOutlined } from '@ant-design/icons';
import type { MenuProps } from 'antd';
import { Dropdown, Space} from 'antd';
import WebSocketComponent from './web_socket';


const DropdownMenu = (props) => {
    const [items, setItems] = useState([]);
    // Update dropdown items whenever data changes
    useEffect(() => {
        const updatedItems = props.signals.map((signalName, index) => ({
            key: index.toString(),
            label: (
                // add checkbox, add to array, on submit send to both dropdown and 
                <p onClick={() => props.setSignal(signalName)}>
                  {signalName}
                </p>
              ),
        }));
        setItems(updatedItems);
    }, [props.signals]);

    return (
        <div>

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
