import React, { useState, useEffect } from 'react';
import { DownOutlined } from '@ant-design/icons';
import { Dropdown, Space, Button } from 'antd';

export interface LiveDropdownMenuProps {
    setSignal: (signals: string[]) => void;
    signals: string[];
    avail: string[];
}

interface MenuItem {
    key: string;
    label: React.ReactNode;
}

const LiveDropdownMenu = (props: LiveDropdownMenuProps) => {
    const [items, setItems] = useState<MenuItem[]>([]);
    const [open, setOpen] = useState<boolean>(false);
    const [selectedSignals, setSelectedSignals] = useState<string[]>([]);

    const handleSignalClick = (signalName: string) => {
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
        const updatedItems = props.avail.map((signalName: string, index: number) => ({
            key: index.toString(),
            label: (
                <Space onClick={() => handleSignalClick(signalName)}>
                    <input
                        style={{ cursor: 'pointer' }}
                        type="checkbox"
                        checked={props.signals.includes(signalName)}
                    />
                    <span>{signalName}</span>
                </Space>
            ),
        }));
        setItems(updatedItems);
    }, [props.avail, props.signals, selectedSignals]);

    return (
        <Dropdown menu={{ items }} open={open}>
            <Button onClick={() => setOpen(!open)} style={{ display: 'block' }}>
                <Space>
                    Signals
                    <DownOutlined />
                </Space>
            </Button>
        </Dropdown>
    );
};

export default LiveDropdownMenu;
