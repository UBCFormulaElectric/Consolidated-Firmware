'use client';

import { useEffect, useState, React } from 'react';
//  WebSockets provide a full-duplex (two-way) communication channel over a single, 
// long-lived connection, making it ideal for real-time data transfer between the client and server.

const WebSocketComponent = (props) => {

    useEffect(() => {
        props.socket.on("available_signals_response", (data) => {
            props.set_available_signals(JSON.parse(data));
        });
    }, [props.socket]);

    const handleSubmit = () => {
        props.socket.emit("signals", {"ids": ["Signal1"]});
    };

    useEffect(() => {
        props.socket.on("signal_response", (data) => {
            props.setData(JSON.parse(data));
        });
    }, [props.socket]);

    return (
        <div>
          <h2>WebSocket Communication</h2>
          <button onClick={handleSubmit}>submit</button>
        </div>
    );
}

export default WebSocketComponent;
