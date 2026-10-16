"use client";

import { API_BASE_URL } from "@/lib/constants";

export default function TransmitPage() {
    return (
        <div className="px-14 gap-8 h-screen flex flex-col pb-20 overflow-hidden">
            <div className="pt-20 w-screen" />
            <span className="text-3xl font-bold">Transmit</span>
            <span className="text-sm text-gray-500">Click the button to send request to endpoint</span>
            <button style={{all: "revert"}} onClick={() => { fetch(`${API_BASE_URL}/api/v1/transmit/ntp`) }}>
                /transmit/ntp
            </button>
            <button style={{all: "revert"}} onClick={() => { fetch(`${API_BASE_URL}/api/v1/transmit/boot_info`) }}>
                /transmit/boot_info
            </button>
        </div>
    );
}
