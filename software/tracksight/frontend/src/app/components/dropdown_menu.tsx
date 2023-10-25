import { useState, useEffect } from 'react'; 
import { DownOutlined, SmileOutlined } from '@ant-design/icons';
import { Dropdown, Space, Button } from 'antd';


// props = url (HTTP url), avail (avail signals), signals (signals to be queried), setSignal (set data) 
const DropdownMenu = (props) => {
    const [items, setItems] = useState([]);
    // for intermediate states of selected signals
    const [open, setOpen] = useState(false);
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
        const updatedItems = props.avail.map((signalName, index) => ({
            key: index.toString(),
            label: (
                <Space onClick={() => handleSignalClick(signalName)} >
                    <input 
                        style={{"cursor": "pointer"}}
                        type="checkbox" 
                        checked={props.signals.includes(signalName)}
                    />
                    <span> 
                        {signalName}
                    </span>
                </Space>
            ),
        }));
        setItems(updatedItems);
    }, [props.avail, props.signals, selectedSignals]);


    return (
            <Dropdown 
                menu={{ items }} 
                open={open}
            >
                <Button onClick={() => setOpen(!open)} style={{"display":"block"}}>
                    <Space>
                        Signals
                        <DownOutlined />
                    </Space>
                </Button>
            </Dropdown>
    );
};

export default DropdownMenu;
