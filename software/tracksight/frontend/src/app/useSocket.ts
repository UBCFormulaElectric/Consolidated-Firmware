'use client';
import { useEffect, useState } from "react";
import { io, Socket } from "socket.io-client";

export const useSocket = (url: string) => {
    const [socket, setSocket] = useState<Socket | null>(null);
    const [loading, setLoading] = useState<boolean>(true);
    
    useEffect(() => {
        if(socket == null) return;
        setLoading(true);
        socket.on("connect", () => {
            setLoading(false);
        });
        socket.on("disconnect", () => {
            console.log("disconnected");
        });
        return () => {
            socket.disconnect();
        };        
    }, [socket])

    useEffect(() => {
        // NOTE -> mac users may need to turn airplay reciever off in order to connect to the server
        setSocket(io(url, {
            transports: ["websocket"],
            transportOptions: {
                origin: new URL(window.location as unknown as string),
            },
        }))
    }, []);

    return {socket, loading};
}