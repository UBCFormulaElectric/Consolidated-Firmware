'use client';

import { Socket } from "socket.io-client";
import React, { Dispatch, SetStateAction, useEffect } from 'react';
import { Button } from 'antd';
//  WebSockets provide a full-duplex (two-way) communication channel over a single, 
// long-lived connection, making it ideal for real-time data transfer between the client and server.


interface Data {
    id: number;
    signals: Record<string, Record<string, number>>;
}

export interface SocketProps {
    id: number,
    socket: Socket,
    setData: Dispatch<SetStateAction<Data>>,
    signals: string[],
}


const WebSocketComponent = (props: SocketProps) => {

const handleSubmit = () => {
  props.socket.emit("signal", {"graph": props.id, "ids": props.signals });  
};

useEffect(() => {
    handleSubmit()
}, []);

    useEffect(() => {
        props.socket.on("signal_response", (data) => {
            props.setData(data);
        });
    }, []);

    return (
        <div>
          <Button onClick={handleSubmit}>submit</Button>
        </div>
    );
}

export default WebSocketComponent;