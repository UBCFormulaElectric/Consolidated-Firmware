

# Module Specifications

*This set of specifications is for the **2019** rules, which may be found [here](http://www.fsaeonline.com/cdsweb/gen/DownloadDocument.aspx?DocumentID=607667ea-bec6-4658-92c4-fff59dbb5c0e)*

This Markdown file contains the specifications for each module. The intent is to have a definitive resource that fully defines all board behaviors, as observed from outside the board.
<<<<<<< HEAD
Each specification has an unique ID, short title, and detailed description. It may also be associated with a competition rule.
=======
Each specification has an unique ID, short title, and detailed description. It may also be associted with a competition rule.
>>>>>>> DCM spec

## Specification Rules
1. Specification ID's should never be duplicated.
1. Specification ID's should only increment. This means that even if have 50 FSM specs and we delete `FSM-23`, the next spec should be `FSM-51`, we should never re-use ID's.
1. Specification descriptions should be unambiguous and formal. Instead of "acceptable value" say "5 V". Instead of "shall send out error constantly", say "shall send out the CAN message `ERR_CAN_MSG` every 4 seconds until the board is reset".
1. All abbreviations (`AIR`, `APPS`, etc.) should be spelled out fully the first time they appear in a given row in the table. This means that if `APPS` appears in `FSM-0` and `FSM-22`, it should be spelled out as `APPS (acceleration pedal position sensor)` in _both_ `FSM-0` and `FSM-22`.

## Background Info
- There are two types of faults, critical and non-critical. Critical faults cause an AIR shutdown, whereas non-critical faults are present for logging and debugging purposes.

## Table Of Contents
- [FSM (Front Sensor Module)](#FSM)
- [DCM (Drive Control Module)](#DCM)
    - [DCM Stateless](#DCM_STATELESS)
    - [DCM Init State](#DCM_INIT)
    - [DCM Drive State](#DCM_DRIVE)
    - [DCM Fault State](#DCM_FAULT)

## FSM <a name="FSM"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
FSM-0 | APPS (Accelerator Pedal Position Sensor) Disagreement | When the ADC readings between the primary and secondary APP's disagree by more then 10%, the FSM shall constantly send the `FSM_AIR_SHUTDOWN` CAN signal with a period of 0.5 seconds until the board is reset. | T.6.2.3, T.6.2.4

## DCM <a name="DCM"></a>

### DCM Stateless <a name="DCM_STATELESS"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
DCM-0 | Startup CAN message | The DCM must transmit a startup message over CAN on boot.
DCM-1 | Brake light control | The DCM must enable the brake light through the corresponding GPIO during brake actuation or regen and must disable the brake light otherwise.
DCM-2 | Heartbeat sending | The DCM must transmit a heartbeat over CAN at 100Hz.
DCM-18 | Heartbeat receiving | The DCM must throw a critical fault once it does not receive three consecutive BMS heartbeats.

### DCM Init State <a name="DCM_INIT"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
DCM-3 | Entering the init state | The DCM state machine must begin in the init state by default.
DCM-4 | In the init state | There are no actions for the DCM to take for this state.
DCM-5 | Exiting the init state and entering the drive state | The DCM must meet the following conditions before entering the drive state: <br/> - There must be no critical faults present on any ECU. <br/> - The shutdown circuit must be closed and precharge must have already occurred. <br/> <br/> The DCM must also meet the following conditions in sequential order before entering the drive state: <br/> 1. The start switch must be switched in an upwards direction. If the start switch was already in the upwards position, it must be re-switched into the upwards position. <br/> 2. The brakes must be actuated. | EV.6.11.2, EV.6.11.3

### DCM Drive State <a name="DCM_DRIVE"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
DCM-6 | Power limiting | The DCM must ensure power draw does not exceed 80kW for more than 100ms continuously or for 500ms over a moving average. | EV.1.3.1, EV.1.4.4
DCM-7 | Torque request limiting | The DCM may only request torque less than or equal to what the driver requested through algorithms such as torque vectoring and traction control. | EV.2.2.3
DCM-8 | Regen at slow speeds | The DCM must disable regen through the DCM's regen outputs when the vehicle is travelling at less than 5 km/hr. | EV.1.2.6
DCM-9 | Drive direction | The DCM must only alter the inverters' forward enable pins. The DCM should never alter the inverters' reverse enable pins, as the vehicle must not drive in reverse. | EV.1.2.7
DCM-10 | AIR shutdown brake torque | The DCM must not request brake torque to the motors when the AIRs are opened during driving. | EV.7.2.9
DCM-11 | Motor shutdown when both pedals are pressed | - The DCM must throw a critical fault after the brake is actuated and the APPS senses more than 25% pedal travel simultaneously. <br/> - The DCM must clear the critical fault after the APPS senses less than 5% pedal travel, regardless of the brake state. | EV.2.4.1, EV.2.4.2
DCM-12 | Entering the drive state from the init state | The DCM must do the following upon entering the drive state: <br/> - Make the ready to drive sound for 2 seconds after entering the drive state. <br/> - Enable both inverters through the DCM's corresponding enable outputs. <br/> - Request through CAN that the inverters send each motor's RPM to the DCM at a rate of 1kHz. | EV.6.11.4, EV.6.11.5
DCM-13 | In the drive state | The DCM must do the following in the drive state at 1kHz: <br/> 1. Acquire each motor's RPM from the inverters. <br/> 2. Map the normalized pedal position to a positive or negative torque request. <br/> 3. Decrease the torque request if necessary to meet both DCM-6 and the maximum positive/negative power limits specified by the BMS. <br/> 4. Send the resultant torque request to both inverters over CAN.
DCM-14 | Exiting the drive state and entering the init state | When the start switch is switched into a downwards position, the DCM must: <br/> - Transition from the drive state to the init state. <br/> - Stop all CAN torque requests to meet DCM-10. <br/> - Disable power to both motors through the DCM's enable pins.

### DCM Fault State <a name="DCM_FAULT"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
DCM-15 | Entering the fault state from any state | The DCM must transition to the fault state whenever a critical fault is observed.
DCM-16 | In the fault state | The DCM must do the following in the fault state at 1kHz: <br/> - Stop all CAN torque requests to meet DCM-4. <br/> - Disable power to both motors through the DCM's enable pins.
DCM-17 | Exiting the fault state and entering the init state | When all critical faults are cleared, re-enter the init state.
