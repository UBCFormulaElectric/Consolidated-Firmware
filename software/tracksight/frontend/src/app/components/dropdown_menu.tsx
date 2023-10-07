import { useState, useEffect } from 'react'; // Removed extra React import
import { DownOutlined, SmileOutlined } from '@ant-design/icons';
import { Dropdown, Space } from 'antd';

const DropdownMenu = (props) => {
    const [items, setItems] = useState([]);
    const [visible, setVisible] = useState(false);
    const [selectedSignals, setSelectedSignals] = useState([]);

    const handleSignalClick = (signalName) => {
        const newSelectedSignals = [...selectedSignals];
        const index = newSelectedSignals.indexOf(signalName);
        if (index === -1) {
            newSelectedSignals.push(signalName);
        } else {
            newSelectedSignals.splice(index, 1);
        }
        setSelectedSignals(newSelectedSignals);
        props.setSignal(newSelectedSignals);
    };

    useEffect(() => {
        const updatedItems = props.signals.map((signalName, index) => ({
            key: index.toString(),
            label: (
                <p>
                    <input 
                        type="checkbox" 
                        checked={selectedSignals.includes(signalName)}
                        onChange={() => handleSignalClick(signalName)} // Added onChange handler
                    />
                    <span onClick={() => handleSignalClick(signalName)}> 
                        {signalName}
                    </span>
                </p>
            ),
        }));
        setItems(updatedItems);
    }, [props.signals, selectedSignals]);

    const handleVisibleChange = (flag) => {
        setVisible(flag);
    };

    return (
        <div className="dropdown" onMouseLeave={() => setVisible(false)}>
            <Dropdown 
                menu={{ items }} 
                open={visible} 
                onOpenChange={handleVisibleChange}
            >
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
