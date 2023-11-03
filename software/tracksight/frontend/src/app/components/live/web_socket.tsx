'use client';


import { useEffect, useState, React } from 'react';
import { Button, Space } from 'antd';
//  WebSockets provide a full-duplex (two-way) communication channel over a single, 
// long-lived connection, making it ideal for real-time data transfer between the client and server.

const WebSocketComponent = (props) => {
const [currId, setCurrId] = useState('');



const handleSubmit = () => {
  setCurrId(props.id);
  console.log(props.signals)
  props.socket.emit("signal", {"graph": props.id, "ids": props.signals });  
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
          <Button onClick={handleSubmit}>submit</Button>
        </div>
    );
}

export default WebSocketComponent;