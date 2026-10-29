"use client";

import { CirclePower, Clock, Save } from "lucide-react";

import EndpointToolCard from "./EndpointToolCard";
import ToolCard from "./ToolCard";

function ToolsPageContent() {
    return (
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
            <ToolCard type="page" title="SD Card Dump" description="Select an SD card and files to dump their contents to the database." href="/sd/dump" icon={<Save className="size-5" />} />
            <EndpointToolCard title="NTP Sync" description="Sync the NTP server with the current system time." endpoint="api/v1/transmit/ntp" icon={<Clock className="size-5" />} />
            <EndpointToolCard title="Boot Info" description="Request boot information from the car." endpoint="api/v1/transmit/boot_info" icon={<CirclePower className="size-5" />} />
        </div>
    );
}

export default ToolsPageContent;
