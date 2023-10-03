'use client';


import { useEffect, useState, React } from 'react';
//  WebSockets provide a full-duplex (two-way) communication channel over a single, 
// long-lived connection, making it ideal for real-time data transfer between the client and server.

const WebSocketComponent = (props) => {
const [message, setMessage] = useState('');

    useEffect(() => {
        props.socket.on("available_signals_response", (data) => {
            console.log(data);
            console.log(typeof data);
            //props.set_available_signals(JSON.parse(data));
        });
    }, [props.socket]);

    const handleSubmit = () => {
        props.socket.emit("signals", { "ids": [props.selectedSignal] });
    };

    useEffect(() => {
        props.socket.on("signal_response", (data) => {
            props.setData(JSON.parse(data));
        });
    }, [props.socket]);

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
