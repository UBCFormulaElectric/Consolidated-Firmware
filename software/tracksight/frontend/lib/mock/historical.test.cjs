const assert = require("node:assert/strict");
const path = require("node:path");
const { test } = require("node:test");
// jiti is already installed by the frontend's tooling; no test framework needed.
const { createJiti } = require("jiti");
const jiti = createJiti(__filename, { alias: { "@": path.resolve(__dirname, "../..") } });

process.env.NEXT_PUBLIC_USE_MOCK_DATA = "true";
global.window = { location: { search: "?mockAlerts=2" } };
global.fetch = async () => {
    throw new Error("Mock mode must not fetch the backend");
};

const { buildMockHistoricalPayload: payload, buildMockHistoricalMarkers } = jiti("./historical.ts");
const { getMockSignalCatalog, getMockAlertNames } = jiti("./catalog.ts");
const { fetchHistoricalSignal } = jiti("../api/historicalSignals.ts");
const { fetchHistoricalMarkers } = jiti("../api/historicalMarkers.ts");
const { fetchHistoricalSessionsForRange } = jiti("../api/historicalSessions.ts");
const catalog = getMockSignalCatalog();
const numerical = catalog.find((signal) => signal.type === "numerical");
const state = catalog.find((signal) => signal.type === "enum");

test("deterministic, source-specific tiles retain backend resolutions and boundaries", () => {
    const start = Date.parse("2026-10-04T09:00:00Z");
    const a = payload(numerical.name, start, start + 2000, "Radio");
    assert.deepEqual(a, payload(numerical.name, start, start + 2000, "Radio"));
    assert.notDeepEqual(a.rows, payload(numerical.name, start, start + 2000, "SdCard").rows);
    const b = payload(numerical.name, start + 500, start + 2500, "Radio");
    const shared = new Map(b.rows.map((row) => [row.timestamp, row.value]));
    for (const row of a.rows) if (shared.has(row.timestamp)) assert.equal(row.value, shared.get(row.timestamp));
    for (const resolution of [10, 20, 50, 100, 500, 1000, 10000, 60000, 600000, 3600000]) {
        const result = payload(numerical.name, start, start + resolution * 256, "Radio");
        assert.equal(result.resolution_ms, resolution);
        assert.ok(result.rows.length <= 1024);
    }
    assert.deepEqual(payload(numerical.name, start, start, "Radio").rows, []);
    assert.deepEqual(payload(numerical.name, start + 1, start, "Radio").rows, []);
});

test("coarse mean/first/max match independently aggregated finest samples", () => {
    for (const signal of [numerical, state, { name: "alert", type: "alert" }]) {
        const raw = new Map();
        for (let start = 0; start < 60000; start += 2000) {
            for (const row of payload(signal.name, start, start + 2000, "Radio").rows) {
                const time = Date.parse(row.timestamp) - 10;
                if (time >= 0 && time < 60000) raw.set(`${row.name}:${time}`, row.value);
            }
        }
        const coarse = payload(signal.name, 0, 256000, "Radio");
        assert.equal(coarse.resolution_ms, 1000);
        for (const row of coarse.rows) {
            const stop = Date.parse(row.timestamp);
            const values = [];
            for (let time = stop - 1000; time < stop; time += 10) {
                const value = raw.get(`${row.name}:${time % 60000}`);
                if (value !== undefined) values.push(value);
            }
            assert.ok(values.length);
            const expected = signal.type === "numerical" ? values.reduce((sum, value) => sum + value, 0) / values.length : signal.type === "alert" ? Math.max(...values) : values[0];
            assert.ok(Math.abs(row.value - expected) < 1e-8, `${row.name} at ${stop}: ${row.value} != ${expected}`);
        }
        assert.equal(
            coarse.rows.some((row) => Date.parse(row.timestamp) > 40000 && Date.parse(row.timestamp) <= 45000),
            false
        );
    }
});

test("historical APIs parse mocks without network and preserve existing shapes", async () => {
    const result = await fetchHistoricalSignal({ signalName: numerical.name, signalType: numerical.type, startUtcMs: 0, endUtcMs: 2000, source: "Radio" });
    assert.deepEqual(Object.keys(result).sort(), ["points", "resolutionMs"]);
    assert.ok(result.points.length);
    for (const point of result.points) {
        assert.deepEqual(Object.keys(point).sort(), ["name", "timestampMs", "value"]);
        assert.ok(Number.isFinite(point.timestampMs) && Number.isFinite(point.value));
        assert.ok(point.value >= numerical.min_val && point.value <= numerical.max_val);
    }
    const alerts = payload("alert", 0, 2000, "Radio");
    assert.deepEqual([...new Set(alerts.rows.map((row) => row.name))], getMockAlertNames(2));
    assert.deepEqual(await fetchHistoricalMarkers(1, 180000, "Radio"), [{ timestampMs: 60000 }, { timestampMs: 120000 }]);
    assert.deepEqual(buildMockHistoricalMarkers(100, 100), []);

    const day = 24 * 60 * 60 * 1000;
    const sessions = await fetchHistoricalSessionsForRange(day, day * 2, "Radio", "UTC");
    assert.deepEqual(
        sessions.map((session) => session.endUtcMs - session.startUtcMs),
        [600000, 10800000, 7200000]
    );
    assert.ok(sessions[2].endUtcMs > day * 2);
    assert.deepEqual(sessions, await fetchHistoricalSessionsForRange(day, day * 2, "Radio", "UTC"));
    assert.deepEqual(await fetchHistoricalSessionsForRange(0, day, "Radio", "UTC"), []);
});

test("delay and failure controls exercise the real async signal API", async () => {
    const params = { signalName: numerical.name, signalType: numerical.type, startUtcMs: 0, endUtcMs: 2000, source: "Radio" };
    global.window.location.search = "?mockDelay=25";
    const start = performance.now();
    await fetchHistoricalSignal(params);
    assert.ok(performance.now() - start >= 20);
    global.window.location.search = "?mockError=1";
    await assert.rejects(fetchHistoricalSignal(params), /Simulated historical signal failure/);
    global.window.location.search = "?mockAlerts=2";
});
