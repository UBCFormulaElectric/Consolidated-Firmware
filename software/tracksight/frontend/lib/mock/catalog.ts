import { BooleanSignalMetadata, EnumSignalMetadata, NumericalSignalMetadata, SignalMetadata, SignalType } from "@/lib/types/Signal";

// Deterministic so signal names survive reloads: saved charts reference signals by name.
function seededRandom(seed: number) {
    return () => {
        seed = (seed + 0x6d2b79f5) | 0;
        let t = Math.imul(seed ^ (seed >>> 15), 1 | seed);
        t = (t + Math.imul(t ^ (t >>> 7), 61 | t)) ^ t;
        return ((t ^ (t >>> 14)) >>> 0) / 4294967296;
    };
}

const NODES = ["BMS", "VC", "FSM", "RSM", "DAM", "CRIT"];
const NUMERICAL_KINDS: { name: string; unit: string; min: number; max: number }[] = [
    { name: "TractiveSystemVoltage", unit: "V", min: 0, max: 600 },
    { name: "TractiveSystemCurrent", unit: "A", min: -50, max: 250 },
    { name: "CellVoltageMin", unit: "V", min: 2.8, max: 4.2 },
    { name: "CellTempMax", unit: "°C", min: 15, max: 65 },
    { name: "WheelSpeed", unit: "km/h", min: 0, max: 120 },
    { name: "MotorTorque", unit: "Nm", min: -20, max: 60 },
    { name: "BrakePressure", unit: "bar", min: 0, max: 80 },
    { name: "SteeringAngle", unit: "deg", min: -120, max: 120 },
    { name: "CoolantTemp", unit: "°C", min: 20, max: 90 },
    { name: "StateOfCharge", unit: "%", min: 0, max: 100 },
    { name: "Throttle", unit: "%", min: 0, max: 100 },
    { name: "LvBatteryVoltage", unit: "V", min: 10, max: 14.5 },
];
const ENUM_KINDS: { name: string; states: string[] }[] = [
    { name: "State", states: ["INIT", "IDLE", "PRECHARGE", "DRIVE", "FAULT"] },
    { name: "DriveMode", states: ["ENDURANCE", "ACCEL", "SKIDPAD", "AUTOCROSS"] },
    { name: "ChargerState", states: ["DISCONNECTED", "CONNECTED", "CHARGING", "DONE"] },
];
const BOOLEAN_KINDS = ["Heartbeat", "FanEnabled", "ContactorClosed", "BrakeLightOn"];

// the curated names keep the default (12) looking like a real car; larger counts are generated
const CURATED_ALERTS = ["BMS_Fault_CellOvertemp", "BMS_Fault_CellUndervoltage", "VC_Fault_InverterRetry", "BMS_Warning_CellImbalance", "BMS_Warning_ChargerDisconnected", "VC_Warning_LowBattery", "FSM_Warning_SteeringAngleOutOfRange", "RSM_Warning_CoolantTempHigh", "VC_Info_RegenDisabled", "BMS_Info_BalancingActive", "DAM_Info_SdCardFull", "CRIT_Info_DriveModeChanged"];
const ALERT_SEVERITIES = ["Fault", "Warning", "Warning", "Info"];

/** Alert names following the real {node}_{Fault|Warning|Info}_{name} convention. */
export function getMockAlertNames(count: number): string[] {
    const names = CURATED_ALERTS.slice(0, count);
    for (let i = names.length; i < count; i++) {
        names.push(`${NODES[i % NODES.length]}_${ALERT_SEVERITIES[i % ALERT_SEVERITIES.length]}_MockAlert${i}`);
    }
    return names;
}

/** A mixed catalog of numerical (~70%), enum (~20%) and boolean (~10%) signals, standing in for /signal/metadata. */
export function getMockSignalCatalog(count: number): SignalMetadata[] {
    const random = seededRandom(count);
    const signals: SignalMetadata[] = [];
    const cycleTimes = [10, 20, 100, 1000];

    for (let i = 0; i < count; i++) {
        const node = NODES[i % NODES.length];
        const round = Math.floor(i / NODES.length);
        const common = { tx_node: node, msg_name: `${node}_MockMessage${Math.floor(i / 4)}`, id: 1000 + i, cycle_time_ms: cycleTimes[Math.floor(random() * cycleTimes.length)] };
        const roll = random();

        if (roll < 0.7) {
            const kind = NUMERICAL_KINDS[round % NUMERICAL_KINDS.length];
            signals.push({ ...common, name: `${node}_${kind.name}${round >= NUMERICAL_KINDS.length ? round : ""}`, type: SignalType.NUMERICAL, min_val: kind.min, max_val: kind.max, unit: kind.unit } satisfies NumericalSignalMetadata);
        } else if (roll < 0.9) {
            const kind = ENUM_KINDS[round % ENUM_KINDS.length];
            const name = `${node}_${kind.name}${round >= ENUM_KINDS.length ? round : ""}`;
            signals.push({ ...common, name, type: SignalType.ENUM, min_val: 0, max_val: kind.states.length - 1, enum_signal: { enum_name: `${node}${kind.name}`, enum_values: Object.fromEntries(kind.states.map((state, value) => [state, value])) } } satisfies EnumSignalMetadata);
        } else {
            const kind = BOOLEAN_KINDS[round % BOOLEAN_KINDS.length];
            signals.push({ ...common, name: `${node}_${kind}${round >= BOOLEAN_KINDS.length ? round : ""}`, type: SignalType.BOOLEAN, min_val: 0, max_val: 1 } satisfies BooleanSignalMetadata);
        }
    }

    // generated names can collide across kinds; keep the first of each
    const seen = new Set<string>();
    return signals.filter((signal) => !seen.has(signal.name) && seen.add(signal.name));
}
