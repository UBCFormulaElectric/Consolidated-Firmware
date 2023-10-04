'use client';


import { useEffect, useState, React } from 'react';
//  WebSockets provide a full-duplex (two-way) communication channel over a single, 
// long-lived connection, making it ideal for real-time data transfer between the client and server.

const WebSocketComponent = (props) => {
const [message, setMessage] = useState('');


    const handleSubmit = () => {
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
    }, [props.socket], [props.setData]);

    useEffect(() => {
        props.socket.on('server_message', (msg) => {
          setMessage(msg);
        });
        
        // Cleanup
        return () => {
          props.socket.off('server_message');
        };
      }, []);
      

    return (
        <div>
          <h2>WebSocket Communication</h2>
          <button onClick={handleSubmit}>submit</button>
        </div>
    );
}

export default WebSocketComponent;
