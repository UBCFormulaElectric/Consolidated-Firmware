import assert from "node:assert/strict";

import { scaleToFitRange } from "./graphViewport.ts";

assert.equal(scaleToFitRange(1070, 1000, 70), 1);
assert.equal(scaleToFitRange(0, 0, 70), 1);
