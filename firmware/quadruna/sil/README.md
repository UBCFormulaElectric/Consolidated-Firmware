# Software In Loop

The vast majority of software we write lives across multiple different binaries, and is deployed on hardware we can't replicate locally on our machines. While simple mocks and unit tests suffice for running tests on a single board, soley relying on unit tests leaves us open to crossboard software faults.

To be able to run the firmware of multiple boards on our local machines, we use our *Software In Loop*.

## Ports
- `3000`) Into a board (rx).
- `3001`) Out of a board (tx).

## Topics
- `time_req`
- `time_resp`
- `can`
- `ready`
- `procedure`

## Navigating Software In Loop Source Code
TODO.