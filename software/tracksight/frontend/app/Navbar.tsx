'use client'

import Link from "next/link";
import socket from "@/lib/realtime/socket";
import { useEffect, useState } from "react";

function Navbar() {

    // const [backendStatus, setBackendStatus] = useState(false);
    const [isConnected, setIsConected] = useState<boolean>(false);

    useEffect(() => {
        socket.on("connect", () => setIsConected(true));
        socket.on("disconnect", () => setIsConected(false));
        socket.on("connect_error", () => setIsConected(false));
    }, []);


    return (
        <nav className="fixed top-0 left-0 z-50 h-min w-screen bg-white border-b border-b-gray-200">
            <div className="flex flex-row items-center gap-6 select-none px-8 py-4">
                <Link href="/">Home</Link>
                <Link href="/live">Live Data</Link>
                <Link href="/historic">Historical Data</Link>
                <div className="ml-auto flex justify-center">
                    {isConnected ?
                        <div className="flex items-center gap-3 px-3 py-2 bg-green-100 border border-gray-200 rounded-lg">
                            <div className="w-2 h-2 bg-green-500 rounded-full" />
                            <div className="text-sm font-medium text-gray-600">Backend OK</div>
                        </div>
                        :
                        <div className="flex items-center gap-3 px-3 py-2 bg-red-100 border border-gray-200 rounded-lg">
                            <div className="w-2 h-2 bg-red-500 rounded-full" />
                            <div className="text-sm font-medium text-gray-600">Backend Disconnected</div>
                        </div>
                    }
                </div>


            </div>
        </nav>
    );
}

export default Navbar;
