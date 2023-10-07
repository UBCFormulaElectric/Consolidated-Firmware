'use client';


import { useEffect, useState, React } from 'react';
import { Button, Space } from 'antd';
//  WebSockets provide a full-duplex (two-way) communication channel over a single, 
// long-lived connection, making it ideal for real-time data transfer between the client and server.

const WebSocketComponent = (props) => {
const [message, setMessage] = useState('');
const [currId, setCurrId] = useState('');



    const handleSubmit = () => {
        setCurrId(props.id)
        props.socket.emit("signal", { "ids": [props.selectedSignal] });
    };

    useEffect(() => {
        props.socket.on("signal_response", (data) => {
            console.log(data);
            if (typeof props.setData === 'function') {
            props.setData(data);
            } else {
                console.error('setData is not a function');
            }
        });
    }, []);

    return (
        <div>
          <h2>WebSocket Communication</h2>
          <Button onClick={handleSubmit}>submit</Button>
        </div>
    );
}

export default WebSocketComponent;
