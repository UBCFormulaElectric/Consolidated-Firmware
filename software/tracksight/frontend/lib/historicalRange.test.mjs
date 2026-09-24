import assert from "node:assert/strict";

import { historicalPointsInRange, inHistoricalRange, rangeOfHistoricalPoints } from "./historicalRange.ts";

const session = { min: 100, max: 200 };
assert.equal(inHistoricalRange(100, session), true);
assert.equal(inHistoricalRange(200, session), false);
assert.equal(inHistoricalRange(NaN, session), false);
const reportedEnd = Date.parse("2026-06-19T12:43:39Z");
assert.equal(inHistoricalRange(Date.parse("2026-06-19T12:43:39.100Z"), { min: reportedEnd - 1000, max: reportedEnd }), false);
assert.deepEqual(
    historicalPointsInRange(
        [
            { timestamp: "2026-06-19T12:43:38.900Z", name: "BMS_TractiveSystemVoltage", value: 2.6 },
            { timestamp: "2026-06-19T12:43:39.100Z", name: "BMS_TractiveSystemVoltage", value: 2.6 },
        ],
        { min: reportedEnd - 1000, max: reportedEnd }
    ),
    [{ timestampMs: reportedEnd - 100, name: "BMS_TractiveSystemVoltage", value: 2.6 }]
);
assert.deepEqual(
    rangeOfHistoricalPoints(
        [
            [{ timestampMs: 90 }, { timestampMs: 120 }],
            [{ timestampMs: 180 }, { timestampMs: 210 }],
        ],
        session
    ),
    { min: 120, max: 180 }
);
assert.equal(rangeOfHistoricalPoints([[{ timestampMs: 90 }, { timestampMs: 210 }]], session), null);
