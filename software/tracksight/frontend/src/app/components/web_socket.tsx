'use client';

import { useEffect, useState, React } from 'react';

const WebSocketComponent = (props) => {

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
