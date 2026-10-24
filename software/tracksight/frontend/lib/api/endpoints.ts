import { API_BASE_URL } from "../constants";

const v1 = `${API_BASE_URL}/api/v1`;
const v2 = `${API_BASE_URL}/api/v2`;

export const ENDPOINTS = {
    signalMetadata: `${v1}/signal/metadata`,
    signalTiles:    (signal: string, start: string, end: string) => `${v1}/signal/tiles/${signal}/${start}/${end}`,
    signalMarkers:  (start: string, end: string) => `${v1}/signal/markers/${start}/${end}`,
    signalSessions: (start: string, end: string) => `${v2}/signal/sessions/${start}/${end}`,
    subscribe:      `${v1}/subscribe`,
    unsubscribe:    `${v1}/unsubscribe`,
    transmitNtp:      `${v1}/transmit/ntp`,
    transmitBootInfo: `${v1}/transmit/boot_info`,
};
