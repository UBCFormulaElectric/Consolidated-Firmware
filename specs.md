# Module Specifications

*This set of specifications is for the **2019** rules, which may be found [here](http://www.fsaeonline.com/cdsweb/gen/DownloadDocument.aspx?DocumentID=607667ea-bec6-4658-92c4-fff59dbb5c0e)*

This Markdown file contains the specifications for each module. The intent is to have a definitive resource that fully defines all board behaviors, as observed from outside the board.
Each specification has an unique ID, short title, and detailed description. It may also be associated with a competition rule.

## Specification Rules
1. Specification ID's should never be duplicated.
1. Specification ID's should only increment. This means that even if have 50 FSM specs and we delete `FSM-23`, the next spec should be `FSM-51`, we should never re-use ID's.
1. Specification descriptions should be unambiguous and formal. Instead of "acceptable value" say "5 V". Instead of "shall send out error constantly", say "shall send out the CAN message `ERR_CAN_MSG` every 4 seconds until the board is reset".
1. All abbreviations (`AIR`, `APPS`, etc.) should be spelled out fully the first time they appear in a given row in the table. This means that if `APPS` appears in `FSM-0` and `FSM-22`, it should be spelled out as `APPS (acceleration pedal position sensor)` in _both_ `FSM-0` and `FSM-22`.

## Background Info
There are two types of faults:
- Critical Faults:
    - These faults are further sub-divided based on how they are handled:
        - Motor shutdown fault: Disable the inverters through a zero torque request
        - AIR shutdown fault: Open the AIRs, then disable the inverters through a zero torque request
- Non-critical:
    - All other faults, for logging and debugging purposes

## Table Of Contents
- [FSM (Front Sensor Module)](#FSM)
    - [FSM Stateless](#FSM_STATELESS)
    - [FSM AIR-Open State](#FSM_AIR_OPEN)
    - [FSM AIR-Closed State](#FSM_AIR_CLOSED)
- [DCM (Drive Control Module)](#DCM)
    - [DCM Stateless](#DCM_STATELESS)
    - [DCM Init State](#DCM_INIT)
    - [DCM Drive State](#DCM_DRIVE)
    - [DCM Fault State](#DCM_FAULT)
- [PDM (Power Distribution Module)](#PDM)
    - [PDM Stateless](#PDM_STATELESS)
    - [PDM Init](#PDM_INIT)
    - [PDM AIR-Open State](#PDM_AIR_OPEN)
    - [PDM AIR-Closed State](#PDM_AIR_CLOSED)
- [BMS (Battery Management System)](#BMS)
    - [BMS Stateless](#BMS_STATELESS)
    - [BMS Init](#BMS_INIT)
    - [BMS Charge State](#BMS_CHARGE)
    - [BMS Drive State](#BMS_DRIVE)
    - [BMS Fault State](#BMS_FAULT)
- [DIM (Dashoard Interface Module)](#DIM)
    - [DIM Stateless](#DIM_STATELESS)
    - [DIM Drive State](#DIM_DRIVE)

## FSM <a name="FSM"></a>

### FSM Stateless <a name="FSM_STATELESS"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
FSM-0 | Startup CAN message | The FSM must transmit a startup message over CAN on boot.
FSM-10 | State CAN message | The FSM must transmit the state of its state machine at 100Hz or faster.
FSM-1 | Heartbeat sending | The FSM must transmit a heartbeat over CAN at 10Hz or faster.
FSM-2 | Heartbeat receiving | The FSM must throw an AIR shutdown fault once it does not receive three consecutive BMS heartbeats.
FSM-3 | Mapped pedal percentage reporting | The FSM must report the mapped pedal percentage over CAN at 100Hz or faster, unless overridden. The FSM must report a mapped pedal percentage of 0% when the brake is actuated.
FSM-4 | Mapped pedal percentage | - The FSM must map the primary APPS position linearly with dead zones on both the low and high ends of the APPS position. <br/> - The low end dead zone boundary must be defined as 3% of the maximum encoder reading when the pedal is completely pressed. <br/> - The high end dead zone boundary must be experimentally determined to ensure the FSM can send 100% mapped pedal percentage despite any mechanical deflection in the pedal box.
FSM-5 | APPS open/short circuit | If there is an open/short circuit in either encoder the FSM must report 0% mapped pedal percentage. | T.4.2.2, T.4.2.9
FSM-6 | APPS disagreement | - When the primary and secondary APPS positions disagree by more than 10%, and the disagreement persists for more than 100ms, the FSM must throw a motor shutdown fault and report 0% mapped pedal percentage. <br/> - The FSM must clear the motor shutdown fault after the APPS positions agree within 10%, and the agreement persists for more than 1s. | T.4.2.3, T.4.2.4, T.4.2.5
FSM-7 | APPS/brake pedal plausibility check | - When the APPS senses brake actuation and more than 25% mapped pedal percentage simultaneously, the FSM must throw a motor shutdown fault and report 0% mapped pedal percentage. <br/> - The FSM must clear the motor shutdown fault after the APPS senses less than 5% mapped pedal percentage, regardless of the brake state. | EV.3.4.1, EV.3.4.2
FSM-8 | Steering angle reporting | - The FSM must report the steering angle in degrees over CAN at 100Hz or faster, where 0 degrees represents straight wheels and a clockwise turn of the steering wheel corresponds to an increase in steering angle. <br/> - The FSM must send a non-critical fault when the steering angle is beyond the maximum and minimum turning radius of the steering wheel.
FSM-9 | Wheel speed reporting | - The FSM must report the two front wheel speeds in km/h over CAN at 100Hz or faster. <br/> - The FSM must send a non-critical fault when either front wheel speed is above 150km/h.
FSM-16 | Open circuit encoder alarm | - When the primary or secondary APPS encoder alarm goes high for 10ms continuously, throw a motor shutdown fault and report 0% mapped pedal percentage. <br/> - When both the primary and secondary APPS encoder alarms go low for 10ms continuously, clear the motor shutdown fault. | T.4.2.9
FSM-18 | Brake actuation reporting | - The FSM must report the brake actuation ON/OFF status over CAN at 100Hz or faster

### FSM AIR-Open State <a name="FSM_AIR_OPEN"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
FSM-17 | Default state | The FSM state machine must begin in the AIR-Open state by default.
FSM-12 | Exiting the AIR-Open state | The FSM must enter the AIR-Closed state when the BMS closes the AIR+ and AIR-.

### FSM AIR-Closed State <a name="FSM_AIR_CLOSED"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
FSM-14 | Coolant flow measurements | - The FSM must measure the coolant flow and apply a heavy low pass filter on the signal (TODO: manually find LPF constant and leave it here). <br/> - If the coolant flow is below the minimum threshold for 1s continuously, the FSM must send a motor shutdown fault. <br/> - If the coolant flow returns above the minimum threshold for 1s continuously, the FSM must clear the motor shutdown fault. <br/> - The FSM must transmit the coolant flow over CAN at 1Hz or faster.
FSM-15 | Exiting the AIR-Closed state | The FSM must enter the AIR-Open state when the BMS open either the AIR+ or AIR-.

## DCM <a name="DCM"></a>

### DCM Stateless <a name="DCM_STATELESS"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
DCM-0 | Startup CAN message | The DCM must transmit a startup message over CAN on boot.
DCM-21 | State CAN message | The DCM must transmit the state of its state machine at 100Hz or faster.
DCM-1 | Brake light control | The DCM must enable the brake light through the corresponding GPIO during brake actuation and/or regen, and must disable the brake light otherwise.
DCM-2 | Heartbeat sending | The DCM must transmit a heartbeat over CAN at 10Hz or faster.
DCM-18 | Heartbeat receiving | The DCM must throw an AIR shutdown fault once it does not receive three consecutive BMS heartbeats.
DCM-19 | Torque Request Reporting | - The DCM must report the torque request over CAN at 100Hz or faster

### DCM Init State <a name="DCM_INIT"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
DCM-4 | Zero torque request sending | The DCM must send zero torque requests to the inverters at 100Hz or faster.
DCM-3 | Default state | The DCM state machine must begin in the init state by default.
DCM-5 | Exiting the init state and entering the drive state | The DCM must meet the following conditions before entering the drive state: <br/> - There must be no critical faults present on any ECU. <br/> - The shutdown circuit must be closed and precharge must have already occurred. <br/> <br/> The DCM must also meet the following conditions in sequential order before entering the drive state: <br/> 1. The start switch must be switched in an upwards direction. If the start switch was already in the upwards position, it must be re-switched into the upwards position. <br/> 2. The brakes must be actuated. | EV.6.11.2, EV.6.11.3

### DCM Drive State <a name="DCM_DRIVE"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
DCM-6 | Power limiting | The DCM must ensure power draw does not exceed 80kW for more than 100ms continuously or for 500ms over a moving average. | EV.1.3.1, EV.2.2.1
DCM-8 | Regen requirements | - Regen is only allowed when the vehicle is travelling more than 5 km/hr and the AIRs are closed. <br/> - Regen is allowed during braking. | EV.1.2.6, EV.8.2.9
DCM-19 | Torque request calculation | - The DCM may only request torque less than or equal to what the driver requested. <br/> - If regen is allowed and the mapped regen paddle percentage is above 0%, the DCM must map the mapped regen paddle percentage to a negative torque request (prioritize regen paddle over accelerator pedal). <br/> - Else, the DCM must map the mapped pedal percentage to a positive torque request. | EV.3.2.3
DCM-9 | Drive direction | The DCM must configure the inverters through CAN to only drive forwards, and never in reverse. | EV.1.2.7
DCM-13 | Torque request sending | The DCM must do the following in the drive state at 100Hz: <br/> 1. Acquire each motor's RPM from the inverters over CAN. <br/> 2. Calculate a torque request as per DCM-19. <br/> 3. Decrease the torque request if necessary to meet both DCM-6 and the maximum power limits specified by the BMS. <br/> 4. Send the resultant torque request to both inverters over CAN.
DCM-12 | Entering the drive state from the init state | The DCM must do the following upon entering the drive state: <br/> - Make the ready to drive sound for 2 seconds after entering the drive state. | EV.7.12.1, EV.7.12.2
DCM-14 | Exiting the drive state and entering the init state | When the start switch is in the downwards position, the DCM must transition from the drive state to the init state.
DCM-20 | Exiting the drive state and entering the fault state | When a motor shutdown is requested over CAN, the DCM must transition from the drive state to the fault state.

### DCM Fault State <a name="DCM_FAULT"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
DCM-16 | Zero torque request sending | The DCM must send zero torque requests at 100Hz or faster.
DCM-17 | Exiting the fault state and entering the init state | When all critical faults are cleared, re-enter the init state.

## PDM <a name="PDM"></a>

### PDM Stateless <a name="PDM_STATELESS"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
PDM-0 | Startup CAN message | The PDM must transmit a startup message over CAN on boot.
PDM-21 | State CAN message | The PDM must transmit the state of its state machine at 100Hz or faster.
PDM-1 | Heartbeat sending | The PDM must transmit a heartbeat over CAN at 10Hz or faster.
PDM-2 | Heartbeat receiving | The PDM must throw an AIR shutdown fault once it does not receive three consecutive BMS heartbeats.
PDM-4 | 18650 charge fault handling | When the CHRG_FAULT GPIO is low (18650s charge fault condition), the PDM must throw a non-critical fault.
PDM-5 | Boost controller fault handling | When the PGOOD GPIO is low (boost controller fault condition), the PDM must throw a non-critical fault.
PDM-6 | Voltage sense rationality checks | The PDM must run voltage rationality checks at 1kHz on the following inputs, throwing a non-critical fault if a rationality check fails: <br/> - VBAT_SENSE: min =  6V, max = 8.5V. <br/> - 24V_AUX_SENSE: min = 22V, max = 26V. <br/> - 24V_ACC_SENSE: min = 22V, max = 26V.
PDM-11 | Current sensing | The PDM must measure and log all e-fuse currents over CAN at 1Hz or faster.
PDM-12 | E-fuse fail-safe mode | If an e-fuse enters fail-safe mode, the PDM must: <br/> - Throw a non-critical fault. <br/> - Control e-fuses over GPIO. <br/> <br/> If the e-fuse recovers, the PDM must return to SPI communication and clear the non-critical fault.
PDM-22 | E-fuse fault behavior | - The PDM must continually compare e-fuse currents against their current limits. <br/> - Once the e-fuse current exceeds the current limit for an output, that output is considered to have faulted, and the PDM must set a non-critical fault. <br/> - Once an e-fuse faults, the PDM should retry by disabling the output for progressively longer times, i.e. time = (retry number * 100ms), then re-enable the output. <br/> - The PDM should retry 5 times before leaving an output permanently off, resetable by GLVMS only. This state is considered unrecoverable, and is handled by PDM-13.
PDM-7 | E-fuse current limits | The PDM's e-fuse current limits must be set to 2.5A for inverter outputs, and 1A for other outputs.
PDM-13 | Unrecoverable e-fuse fault behavior | The PDM must throw a motor shutdown or non-critical fault (different than the non-critical fault in PDM-22) over CAN depending on the e-fuse in the fault state: <br/> - AUX 1: Non-critical. <br/> - AUX 2: Non-critical. <br/> - Drive Inverter Left: Motor ([TODO: if time permits, run on one inverter](https://github.com/UBCFormulaElectric/Consolidated-Firmware/issues/514)). <br/> - Drive Inverter Right: Motor. <br/> - Energy Meter: Motor. <br/> - CAN: Motor. <br/> - AIR SHDN: Motor.

### PDM Init State <a name="PDM_INIT"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
PDM-21 | E-fuse disabling | The PDM must disable all e-fuses in the init state.
PDM-8 | Default state | The PDM state machine must begin in the init state by default.
PDM-10 | Exiting the init state and entering the AIR-Open state | After the PDM is finished programming the e-fuses, the PDM must enter the AIR-Open state.

### PDM AIR-Open State <a name="PDM_AIR_OPEN"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
PDM-16 | Selective e-fuse enabling | The PDM must only enable the following e-fuse outputs in the AIR-Open state: AUX 1, AUX 2, Energy Meter, CAN, AIR SHDN
PDM-15 | Entering the AIR-Open state | The PDM must enter the AIR-Open state after the init state is complete.
PDM-17 | Exiting the AIR-Open state | The PDM must enter the AIR-Closed state when the BMS closes the AIR+ and AIR-.

### PDM AIR-Closed State <a name="PDM_AIR_CLOSED"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
PDM-19 | Selective e-fuse enabling | The PDM must enable all e-fuse outputs in the AIR-Closed state, except for the inverters, which depend on the following:. <br/> <br/> If the start switch is off AND the car is travelling slower than 10km/h, both inverters must be disabled. Otherwise, both inverters must be enabled, to protect against BEMF.
PDM-20 | Exiting the AIR-Closed state | The PDM must enter the AIR-Open state when the BMS opens the AIR+ or AIR-.

## BMS <a name="BMS"></a>

### BMS Stateless <a name="BMS_STATELESS"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
BMS-0 | Startup CAN message | The BMS must transmit a startup message over CAN on boot.
BMS-31 | State CAN message | The BMS must transmit the state of its state machine at 100Hz or faster.
BMS-1 | Heartbeat sending | The BMS must transmit a heartbeat over CAN at 10Hz or faster.
BMS-2 | Heartbeat receiving | - The BMS must throw an AIR shutdown fault and enter the fault state once it does not receive three consecutive FSM or DCM heartbeats. <br/> - The BMS must throw a non-critical fault once it does not receive three consecutive PDM heartbeats.
BMS-3 | isoSPI communication failure | - Upon isoSPI communication that results in a packet error code (PEC) mismatch, the BMS must retry communication. <br/> - After three consecutive unsuccessful isoSPI communication attempts, the BMS must throw an AIR shutdown fault and enter the fault state.
BMS-4 | Cell voltages acquisition and logging | - The BMS must acquire all cell voltages over isoSPI at 100Hz. <br/> - The BMS must log the highest cell, lowest cell, average cell, pack, and segment voltages at 100Hz or faster.
BMS-5 | Cell temperatures acquisition and logging | - The BMS must acquire all cell temperatures over isoSPI at 1Hz. <br/> - The BMS must log the highest, lowest and average temperatures at 1Hz or faster.
BMS-6 | General voltage and temperature limits | The BMS must throw an AIR shutdown fault and enter the fault state outside of these bounds: <br/> 3.0 < any cell voltage < 4.2V. <br/> -20.0C < any cell temperature < 60.0C. | EV.5.1.3, EV.5.1.10
BMS-7 | Charge temperature limits | The BMS must throw an AIR shutdown fault and enter the fault state if charging is attempted outside of these bounds: <br/>  0.0C < any cell temperature < 45.0C. | EV.5.1.3, EV.5.1.10
BMS-9 | Charger detection and logging | - The BMS must check the charger connection status at 1Hz by the state of the CHARGE_STATE_3V3 digital input. <br/> - The BMS must log the charger connection status over CAN at 1Hz or faster.
BMS-10 | Charger enable/disable | The BMS must enable the charger by setting the BMS PON pin high and disable the charger by setting the BMS PON pin low.
BMS-11 | AIRs weld/stuck open detection | The BMS must check that the AIR+ and AIR- are in the desired open or closed state at 100Hz, and if not the BMS must throw an AIR shutdown fault and enter the fault state.
BMS-36 | IMD data transmission | The BMS must transmit the high/low status of the IMD's OK_HS output, the information encoded in the IMD's PWM output, and the seconds elapsed since the IMD was powered on.
BMS-37 | OK status transmission | The BMS must transmit the on/off status of BMS_OK, IMD_OK and BSPD_OK at 100Hz or faster.
BMS-38 | AIR states transmission | The BMS must transmit the open/closed states of the AIR+ and AIR- over CAN at 100Hz or faster. The open/closed states of the AIR+ and AIR- is determined by the AIR_POWER_STATUS and AIR_TOTAL_ISENSE.
                                                                                                                                                                                              
### BMS Init State <a name="BMS_INIT"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
BMS-12 | Precharge | - The BMS must wait for 5 seconds after boot, then wait for the closing of the AIR-, to execute the precharge sequence. <br/> - The BMS must precharge the inverter/charger capacitors to at least 98% of the accumulator voltage for extra safety margin. <br/> - Upon a successful precharge, the BMS must close the AIR+. <br/> <br/> Upon a precharge failure, the BMS must throw an AIR shutdown fault. A precharge failure occurs when: <br/> - The TS (tractive system) bus voltage does not rise within the allotted time. <br/> - The TS bus voltage rises too quickly. ([TODO: calculate constants](https://github.com/UBCFormulaElectric/Consolidated-Firmware/issues/515))| EV.6.9.1
BMS-35 | SoC retrieval | The BMS must retrieve SoC from three different EEPROM regions, and use a voting algorithm to identify which data is correct, in case of data corruption. In the case of data corruption, set the SoC to 50%. 
BMS-13 | Default State | The BMS state machine must begin in the init state by default.
BMS-15 | Exiting the init state and entering the charge state | Upon a successful precharge, the BMS must enter the charge state if the charger is connected.
BMS-16 | Exiting the init state and entering the drive state | Upon a successful precharge, the BMS must enter the drive state if the charger is disconnected.

### BMS Charge State <a name="BMS_CHARGE"></a>

ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
BMS-34 | SoC calculation and storage | - The BMS must transmit SoC over CAN at 100Hz or faster. <br/> - The BMS must perform coulomb counting and calculate SoC at 100Hz. <br/> The BMS must store the same SoC value in three different EEPROM memory sections at 1Hz. <br/> - SoC must be bounded between 0% and 100%. <br/> - After charging is complete, the BMS must reset and store SoC as 100%.
BMS-17 | Charging thermal safety | - The BMS must stop cell balancing once the LTC6813 internal die temperature (ITMP) exceeds 115C and throw a non-critical fault. The BMS must re-enable cell balancing once the ITMP decreases below 110C. <br/> - The BMS must disable the charger once the ITMP exceeds 120C and throw a non-critical fault. The BMS must re-enable the charger once the ITMP decreases below 115C. <br/>  - The BMS must disable the charger when any cell temperature exceeds 43C and throw a non-critical fault. The BMS must re-enable the charger once the highest cell temperature is below 40C. <br/> - The BMS must throw an AIR shutdown fault and enter the fault state if any cell temperature exceeds 45C. | EV.5.1.3
BMS-18 | Cell balancing | - The BMS must balance the cells until they are all between 4.19V and 4.2V. <br/> - The BMS must only perform cell balancing when the AIRs are closed. <br/> - The BMS must charge and cell balance simultaneously to get all cells charged and balanced as fast as possible.| EV.7.2.5
BMS-19 | Power limits calculation and sending (charge state) | - The BMS must calculate charge power limits based on cell temperatures and SoC to avoid exceeding a cell's defined limits. <br/> - The BMS must send the charge power limits to the charger over CAN at 100Hz or faster.
BMS-20 | Charger disconnection | Upon sensing charger disconnection, the BMS must throw an AIR shutdown fault and enter the fault state.
BMS-21 | Entering the charge state | The BMS must only enter the charge state after the init state is complete.
BMS-23 | Exiting the charge state and entering the init state | Once charging is complete, the BMS must disable the charger, disable cell balancing, open the AIR+, and enter the init state.
BMS-24 | Exiting the charge state and entering the fault state | The BMS must disable cell balancing and charging.

### BMS Drive State <a name="BMS_DRIVE"></a>

ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
BMS-32 | SoC calculation and storage | - The BMS must transmit SoC over CAN at 100Hz or faster. <br/> - The BMS must perform coulomb counting and calculate SoC at 100Hz. <br/> The BMS must store SoC in EEPROM at 1Hz in three different memory locations. <br/> - SoC must be bounded between 0% and 100%.
BMS-26 | Power limits calculation and sending (drive state) | - The BMS must calculate charge and discharge power limits based on cell temperatures and SoC to avoid exceeding a cell's defined limits. <br/> - The BMS must send the charge and discharge power limits over CAN at 100Hz or faster.
BMS-25 | Entering the drive state | The BMS must only enter the drive state from the init state after precharge or from the motor shutdown fault state after faults are cleared.
BMS-27 | Exiting the drive state and entering the init state | Upon the opening of the AIR+ or AIR- outside of an AIR shutdown fault, the BMS must exit the drive state and enter the init state.
BMS-20 | Exiting the drive state and entering the fault state | When an AIR shutdown is requested over CAN, the BMS must transition from the drive state to the fault state.

### BMS Fault State <a name="BMS_FAULT"></a>

ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
BMS-29 | Entering the fault state | The BMS must open the AIR+. 
BMS-30 | Exiting the fault state and entering the init state | Once all AIR shutdown faults are cleared, the BMS must exit the fault state and enter the init state.

## DIM <a name="DIM"></a>

### DIM Stateless <a name="DIM_STATELESS"></a>

ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
DIM-0 | Startup CAN message | The DIM must transmit a startup message over CAN on boot.
DIM-12 | State CAN message | The DIM must transmit the state of its state machine at 100Hz or faster.
DIM-10 | Heartbeat sending | The DIM must transmit a heartbeat over CAN at 10Hz or faster.
DIM-1 | Heartbeat receiving | The DIM must set the 7-segments all on to display '888' once it does not receive three consecutive BMS heartbeats.

### DIM Drive State <a name="DIM_DRIVE"></a>

ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
DIM-11 | Default state | The DIM state machine must begin in the drive state by default.
DIM-2 | Board status LEDs | The DIM must indicate the current status of the BMS, DCM, DIM, FSM and PDM using RGB LEDs, where GREEN = no fault, BLUE = non-critical fault and RED = critical fault. | EV.6.1.11
DIM-3 | Drive mode switch | The DIM must transmit the drive mode position of the rotary switch over CAN at 100Hz or faster.
DIM-4 | Start, traction control, torque vectoring switches | For each of the switches, the DIM must: <br/> - Transmit the on/off switch status of over CAN at 100Hz or faster. <br/> - Set the corresponding green status LEDs when the switch is on.
DIM-5 | IMD LED | The DIM must turn on the IMD LED when it receives IMD fault status from BMS over CAN. | EV.8.5.5
DIM-6 | BSPD LED | The DIM must turn on the BSPD LED when it receives BSPD fault status from FSM over CAN.
DIM-7 | Mapped regen paddle percentage reporting | The DIM must report the mapped regen paddle percentage over CAN at 100Hz or faster.
DIM-8 | Mapped regen paddle percentage | The DIM must linearly map the regen paddle position with dead zones as a percentage (paddle <=5% depressed maps to 0% regen, paddle >=95% pressed maps to 100% regen).
DIM-9 | 7-segment |  - If no faults are present, the DIM must display the SoC as percentage on the 7-segment displays. <br/> - If any fault is present, the DIM must stop displaying the SoC and instead display the fault(s) on the 7-segment displays. <br/> - Each fault should be uniquely identified by an ID that is greater than or equal to 500 so it will not be confused with the SoC. <br/> - If there are more than one fault present, the DIM must cycle through displaying each present fault at 1Hz. </ul>
