import { useEffect, useState } from 'react'

const WebSocket = (socket) => {
    const [message, setMessage] = useState("");
    const [messages, setMessages] = useState([]);

    const handleSubmit = () => {
        if (!message) return;
        socket.emit("data", message);
        setMessage("");
    };

    useEffect(() => {
        socket.on("data", (data) => {
            setMessages([...messages, data.data]);
        });
    }, [socket, messages]);

    return (
        <div>
          <h2>WebSocket Communication</h2>
          <input type="text" value={message} onChange={(e} => setMessage(e.target.value);} />
          <button onClick={handleSubmit}>submit</button>
          <ul>
            {messages.map((message, ind) => {
              return <li key={ind}>{message}</li>;
            })}
          </ul>
        </div>
    );
}
