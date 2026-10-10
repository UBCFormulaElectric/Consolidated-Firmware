# Historical UI testing

Run the frontend with `NEXT_PUBLIC_USE_MOCK_DATA=true` (no backend needed).
Open `/historical?mockCharts=20&mockAlerts=12&mockDelay=300`, then select a session.
Use 5, 20, or 50 charts; `mockCharts` is removed after saving the generated layout.
Add `mockError=1` to exercise signal/alert failures. Delay/error controls affect
signal requests, so the session picker remains usable.

Samples repeat a deterministic minute: waves, ramps, flat sections, spikes,
five-second dropouts, state transitions, and overlapping alert pulses. Radio and
SD-card values differ. Requested tiles use the backend's resolution ladder,
alignment, bucket-end timestamps, numerical means, state firsts, and alert maxima.
Sessions include ten-minute, three-hour, and overnight examples; every third UTC
day is empty. Markers appear each minute. Existing API types remain unchanged.

For comparable timings, run `NEXT_PUBLIC_USE_MOCK_DATA=true yarn build` and
`yarn start`, reuse the saved chart layout and session, then repeat zoom, pan,
hover, and source switching. Watch the navbar's FPS/worst-frame readout and record
browser Performance/Memory traces. The mock measures frontend work, not database
or network throughput; its short repeating pattern avoids generating hours of
raw data. The samples/s counter includes points processed by historical merges,
including overlapping responses.

Run the checks with `node --test lib/mock/historical.test.cjs` after installing
the frontend dependencies. They verify deterministic tiles, source separation,
aggregation against finest samples, empty ranges, API shapes, sessions, markers,
and delay/error controls.
