
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
        - AIR shutdown: requires an AIR shutdown and a motor shutdown
        - Motor shutdown: requires a motor shutdown only
    - Non-critical:
        - All other faults, for logging and debugging purposes

## Table Of Contents
- [FSM (Front Sensor Module)](#FSM)
- [DCM (Drive Control Module)](#DCM)
    - [DCM Stateless](#DCM_STATELESS)
    - [DCM Init State](#DCM_INIT)
    - [DCM Drive State](#DCM_DRIVE)
    - [DCM Fault State](#DCM_FAULT)
- [PDM (Power Distribution Module)](#PDM)
    - [PDM Stateless](#PDM_STATELESS)
    - [PDM Run State](#PDM_RUN)
- [BMS (Battery Management System)](#BMS)
    - [BMS Stateless](#BMS_STATELESS)
    - [BMS Charging State](#BMS_CHARGING)
    - [BMS Driving State](#BMS_DRIVING)
    - [BMS Motor Shutdown Fault State](#BMS_MOTOR_SHUTDOWN_FAULT)
    - [BMS AIR Shutdown Fault State](#BMS_AIR_SHDN_FAULT)
- [DIM (Dashoard Interface Module)](#DIM)

## FSM <a name="FSM"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
FSM-0 | Startup CAN message | The FSM must transmit a startup message over CAN on boot.
FSM-1 | Heartbeat sending | The FSM must transmit a heartbeat over CAN at 100Hz.
FSM-2 | Heartbeat receiving | The FSM must throw an AIR shutdown fault once it does not receive three consecutive BMS heartbeats.
FSM-3 | APPS (accelerator pedal position sensor) reporting | The FSM must report the APPS mapping over CAN at 100Hz, unless overridden.
FSM-4 | APPS mapping | - The FSM must map the primary APPS position to pedal travel percentage linearly with dead zones on both the low and high ends of the APPS position. <br/> - The low end dead zone boundary must be defined as 1.5 multiplied by the maximum encoder reading when the pedal is completely depressed. <br/> - The high end dead zone boundary must be experimentally determined to ensure the FSM can send 100% pedal travel despite any mechanical deflection in the pedal box.
FSM-5 | APPS open/short circuit | If there is an open/short circuit in either encoder the FSM must report 0% pedal travel. | T.6.2.2, T.6.2.9
FSM-6 | APPS disagreement | When the primary and secondary APPS positions disagree by more then 10%, the FSM must throw a motor shutdown fault and report 0% pedal travel. | T.6.2.3, T.6.2.4
FSM-7 | APPS/brake pedal plausibility check | - When the APPS senses brake actuation and more than 25% pedal travel simultaneously, the FSM must throw a motor shutdown fault and report 0% pedal travel. <br/> - The FSM must clear the motor shutdown fault after the APPS senses less than 5% pedal travel, regardless of the brake state. | EV.2.4.1, EV.2.4.2
FSM-8 | Steering angle reporting | - The FSM must report the steering angle in degrees over CAN at 100Hz, where 0 degrees represents straight wheels and a clockwise turn of the steering wheel corresponds to an increase in steering angle. <br/> - The FSM must send a non-critical fault when the steering angle is beyond the max turning radius of the steering wheel.
FSM-9 | Wheel speed reporting | - The FSM must report the two front wheel speeds in km/h over CAN at 100Hz. <br/> - The FSM must send a non-critical fault when either front wheel speed is below -10km/h or above 150km/h.

## DCM <a name="DCM"></a>

### DCM Stateless <a name="DCM_STATELESS"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
DCM-0 | Startup CAN message | The DCM must transmit a startup message over CAN on boot.
DCM-1 | Brake light control | The DCM must enable the brake light through the corresponding GPIO during brake actuation or regen and must disable the brake light otherwise.
DCM-2 | Heartbeat sending | The DCM must transmit a heartbeat over CAN at 100Hz.
DCM-18 | Heartbeat receiving | The DCM must throw an AIR shutdown fault once it does not receive three consecutive BMS heartbeats.

### DCM Init State <a name="DCM_INIT"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
DCM-3 | Entering the init state | The DCM state machine must begin in the init state by default.
DCM-4 | In the init state | There are no actions for the DCM to take for this state.
DCM-5 | Exiting the init state and entering the drive state | The DCM must meet the following conditions before entering the drive state: <br/> - There must be no AIR shutdown or motor shutdown faults present on any ECU. <br/> - The shutdown circuit must be closed and precharge must have already occurred. <br/> <br/> The DCM must also meet the following conditions in sequential order before entering the drive state: <br/> 1. The start switch must be switched in an upwards direction. If the start switch was already in the upwards position, it must be re-switched into the upwards position. <br/> 2. The brakes must be actuated. | EV.6.11.2, EV.6.11.3

### DCM Drive State <a name="DCM_DRIVE"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
DCM-6 | Power limiting | The DCM must ensure power draw does not exceed 80kW for more than 100ms continuously or for 500ms over a moving average. | EV.1.3.1, EV.1.4.4
DCM-7 | Torque request limiting | The DCM may only request torque less than or equal to what the driver requested through algorithms such as torque vectoring and traction control. | EV.2.2.3
DCM-8 | Regen at slow speeds | The DCM must disable regen through the DCM's regen outputs when the vehicle is travelling at less than 5 km/hr. | EV.1.2.6
DCM-9 | Drive direction | The DCM must only alter the inverters' forward enable pins. The DCM should never alter the inverters' reverse enable pins, as the vehicle must not drive in reverse. | EV.1.2.7
DCM-10 | AIR shutdown brake torque | The DCM must not request brake torque to the motors when the AIRs are opened during driving. | EV.7.2.9
DCM-12 | Entering the drive state from the init state | The DCM must do the following upon entering the drive state: <br/> - Make the ready to drive sound for 2 seconds after entering the drive state. <br/> - Enable both inverters through the DCM's corresponding enable outputs. <br/> - Request through CAN that the inverters send each motor's RPM to the DCM at a rate of 1kHz. | EV.6.11.4, EV.6.11.5
DCM-13 | In the drive state | The DCM must do the following in the drive state at 1kHz: <br/> 1. Acquire each motor's RPM from the inverters. <br/> 2. Map the normalized pedal position to a positive or negative torque request. <br/> 3. Decrease the torque request if necessary to meet both DCM-6 and the maximum positive/negative power limits specified by the BMS. <br/> 4. Send the resultant torque request to both inverters over CAN.
DCM-14 | Exiting the drive state and entering the init state | When the start switch is switched into a downwards position, the DCM must: <br/> - Transition from the drive state to the init state. <br/> - Stop all CAN torque requests to meet DCM-10. <br/> - Disable power to both motors through the DCM's enable pins.

### DCM Fault State <a name="DCM_FAULT"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
DCM-15 | Entering the fault state from any state | The DCM must transition to the fault state whenever an AIR shutdown or motor shutdown fault is observed.
DCM-16 | In the fault state | The DCM must do the following in the fault state at 1kHz: <br/> - Stop all CAN torque requests to meet DCM-4. <br/> - Disable power to both motors through the DCM's enable pins.
DCM-17 | Exiting the fault state and entering the init state | When all AIR shutdown and motor shutdown faults are cleared, re-enter the init state.

## PDM <a name="PDM"></a>

### PDM Stateless <a name="PDM_STATELESS"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
PDM-0 | Startup CAN message | The PDM must transmit a startup message over CAN on boot.
PDM-1 | Heartbeat sending | The PDM must transmit a heartbeat over CAN at 100Hz.
PDM-2 | Heartbeat receiving | The PDM must throw an AIR shutdown fault once it does not receive three consecutive BMS heartbeats.
PDM-3 | 18650 overvoltage handling | When the 24V systems are powered by the 18650s and the OV_FAULT GPIO is low (18650 overvoltage fault condition), the PDM must throw an AIR shutdown fault.
PDM-4 | 18650 charge fault handling | When the CHRG_FAULT GPIO is low (18650s charge fault condition), the PDM must throw a non-critical fault.
PDM-5 | Boost controller fault handling | When the PGOOD GPIO is low (boost controller fault condition), the PDM must throw a non-critical fault.
PDM-6 | Voltage sense rationality checks | The PDM must run voltage rationality checks at 1kHz on the following inputs, throwing a non-critical fault if a rationality check fails: <br/> - VBAT_SENSE: min =  6V, max = 8.5V. <br/> - 24V_AUX_SENSE: min = 22V, max = 26V. <br/> - 24V_ACC_SENSE: min = 22V, max = 26V.

### PDM Init State <a name="PDM_INIT"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
PDM-7 | E-fuse current limits | The PDM's e-fuse current limits are as follows: <br/> - 3A for the coolant pump output. <br/> - 1A for all other outputs.
PDM-8 | Entering the init state | The PDM state machine must begin in the init state by default.
PDM-9 | In the init state | - The PDM must program the e-fuses with the current limits listed in PDM-7 over SPI. <br/> - The PDM must enable auto-retry on all e-fuses over SPI.
PDM-10 | Exiting the init state and entering the run state | After the PDM is finished programming the e-fuses, the PDM must enter the run state.

### PDM Run State <a name="PDM_RUN"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
PDM-11 | Current sensing | The PDM must log all e-fuse currents over CAN at 1Hz. This involves, for each e-fuse (and its corresponding channel): <br/> 1. Waiting for a falling edge on the SYNC pin. <br/> 2. Reading the CSNS pin and converting it to a current.
PDM-12 | E-fuse fail-safe mode | The PDM must check if it cannot communicate with an e-fuse over SPI, or if the PDM regains SPI communication with an e-fuse and detects it in fail-safe mode, at 1kHz. If either of these cases are true: <br/> - The PDM must throw a non-critical fault. <br/> - If the PDM has re-gained SPI communication, the PDM must put the e-fuse back into normal mode over SPI.
PDM-13 | E-fuse fault mode | The PDM must check if each e-fuse enters fault mode at 1kHz over SPI. The PDM must throw an AIR shutdown or non-critical fault over CAN depending on the e-fuse in the fault state: <br/> - AUX 1: Non-critical. <br/> - AUX 2: Non-critical. <br/> - Drive Inverter Left: Non-critical. <br/> - Drive Inverter Right: Non-critical. <br/> - Cooling: Critical. <br/> - Energy Meter: Non-critical. <br/> - CAN: Critical. <br/> - AIR SHDN: Critical.
PDM-14 | E-fuse fault delatching | After an e-fuse has faulted and completed its auto-retry sequence, the PDM must make three attempts to delatch the fault over SPI and wait 1s in between attempts. If the e-fuse's fault is cleared, clear the corresponding fault over CAN.
PDM-15 | Entering the run state | The PDM state machine must enter the run state after the init state is complete.
PDM-16 | In the run state | The PDM must perform PDM-11, PDM-12, PDM-13 and PDM-14 in the run state.
PDM-17 | Exiting the run state | The PDM must never exit the run state after entering the run state.

## BMS <a name="BMS"></a>

### BMS Stateless <a name="BMS_STATELESS"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
BMS-0 | Startup CAN message | The BMS must transmit a startup message over CAN on boot.
BMS-1 | Heartbeat sending | The BMS must transmit a heartbeat over CAN at 100Hz.
BMS-2 | Heartbeat receiving | - The BMS must throw an AIR shutdown fault and enter the AIR shutdown fault state once it does not receive three consecutive FSM or DCM heartbeats. <br/> - The BMS must throw a non-critical fault once it does not receive three consecutive PDM heartbeats.
BMS-3 | isoSPI communication failure | - Upon isoSPI communication that results in a packet error code (PEC) mismatch, the BMS must retry communication. <br/> - After three consecutive unsuccessful isoSPI communication attempts, the BMS must throw an AIR shutdown fault and enter the AIR shutdown fault state.
BMS-4 | Cell voltages acquisition and logging | - The BMS must acquire all cell voltages over isoSPI at 100Hz. <br/> - The BMS must log the highest cell, lowest cell, average cell, pack, and segment voltages at 100Hz.
BMS-5 | Cell temperatures acquisition and logging | - The BMS must acquire all cell temperatures over isoSPI at 1Hz. <br/> - The BMS must log the highest, lowest and average temperatures at 1Hz.
BMS-6 | General voltage and temperature limits | The BMS must throw an AIR shutdown fault and enter the AIR shutdown fault state outside of these bounds: <br/> 3.0 < any cell voltage < 4.2V. <br/> -20.0C < any cell temperature < 60.0C. | EV.5.1.3, EV.5.1.10
BMS-7 | Charge temperature limits | The BMS must throw an AIR shutdown fault and enter the AIR shutdown fault state if charging is attempted outside of these bounds: <br/>  0.0C < any cell temperature < 45.0C. | EV.5.1.3, EV.5.1.10
BMS-9 | Charger detection and logging | - The BMS must check the charger connection status at 1Hz by the state of the CHARGE_STATE_3V3 digital input. <br/> - The BMS must log the charger connection status over CAN at 1Hz.
BMS-10 | Charger enable/disable | The BMS must enable the charger by setting the BMS PON pin high and disable the charger by setting the BMS PON pin low.
BMS-11 | Contactor weld/stuck open detection | The BMS must check that the contactors are in the desired open or closed state at 1kHz, and if not the BMS must throw an AIR shutdown fault and enter the AIR shutdown fault state.

### BMS Init State <a name="BMS_INIT"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
BMS-12 | Precharge | The BMS must precharge the inverter/charger capacitors to at least 98% of the accumulator voltage for extra safety margin. <br/> Upon a successful precharge, the BMS must close the AIR+ contactor. <br/> A precharge failure occurs when: <br/> - The TS (tractive system) bus voltage does not rise within the allotted time. <br/> - The TS bus voltage rises too quickly. | EV.6.9.1
BMS-13 | Entering the init state | The BMS state machine must begin in the init state by default.
BMS-14 | In the init state | The BMS must wait for the closing of the AIR- contactor, indicated by a rising edge on the AIR_POWER_STATUS digital input, to execute the precharge sequence.
BMS-15 | Exiting the init state and entering the charging state | Upon a successful precharge, the BMS must enter the charging state if the charger is connected.
BMS-16 | Exiting the init state and entering the driving state | Upon a successful precharge, the BMS must enter the driving state if the charger is disconnected.

### BMS Charging State <a name="BMS_CHARGING"></a>

ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
BMS-17 | Charging thermal safety | - The BMS must stop cell balancing once the LTC6813 internal die temperature (ITMP) exceeds 115C and throw a non-critical fault. The BMS must re-enable cell balancing once the ITMP decreases below 110C. <br/> - The BMS must disable the charger once the ITMP exceeds 120C and throw a non-critical fault. The BMS must re-enable the charger once the ITMP decreases below 115C. <br/>  - The BMS must disable the charger when any cell temperature exceeds 43C and throw a non-critical fault. The BMS must re-enable the charger once the highest cell temperature is below 40C. <br/> - The BMS must throw an AIR shutdown fault and enter the AIR shutdown fault state if any cell temperature exceeds 45C. | EV.5.1.3
BMS-18 | Cell balancing | - The BMS must balance the cells until they are all between 4.19V and 4.2V. <br/> - The BMS must only perform cell balancing when the AIRs are closed. | EV.7.2.5
BMS-19 | Power limits calculation and sending (charging state) | - The BMS must calculate charge power limits based on cell temperatures and SoC to avoid exceeding a cell's defined limits. <br/> - The BMS must send the charge power limits to the charger over CAN at 1kHz.
BMS-20 | Charger disconnection | Upon sensing charger disconnection, the BMS must throw an AIR shutdown fault and enter the AIR shutdown fault state.
BMS-21 | Entering the charging state | The BMS must only enter the charging state after the init state is complete.
BMS-22 | In the charging state | The BMS must charge and cell balance simultaneously to get all cells charged and balanced as fast as possible.
BMS-23 | Exiting the charging state and entering the init state | Once charging is complete, the BMS must disable the charger, disable cell balancing, open the contactors and enter the init state.
BMS-24 | Exiting the charging state and entering the AIR shutdown fault state | The BMS must disable cell balancing and charging.

### BMS Driving State <a name="BMS_DRIVING"></a>

ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
BMS-25 | Entering the driving state | The BMS must only enter the driving state from the init state after precharge or from the motor shutdown fault state after faults are cleared.
BMS-26 | Power limits calculation and sending (driving state) | - The BMS must calculate charge and discharge power limits based on cell temperatures and SoC to avoid exceeding a cell's defined limits. <br/> - The BMS must send the charge and discharge power limits to the DCM over CAN at 1kHz.
BMS-27 | Exiting the driving state and entering the init state | Upon the opening of the contactors outside of an AIR shutdown fault, the BMS must exit the driving state and enter the init state.

### BMS Motor Shutdown Fault State <a name="BMS_MOTOR_SHUTDOWN_FAULT"></a>

ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
BMS-28 | Exiting the motor shutdown state | - Upon an AIR shutdown fault, the BMS must exit the motor shutdown fault state and enter the AIR shutdown fault state. <br/> - Once all motor shutdown faults are cleared, the BMS must exit the motor shutdown state and re-enter the previous state (init or driving).

### BMS AIR Shutdown Fault State <a name="BMS_AIR_SHDN_FAULT"></a>

ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
BMS-29 | Entering the AIR shutdown state | The BMS must open both contactors.
BMS-30 | Exiting the AIR shutdown state and entering the init state | Once all motor shutdown and AIR shutdown faults are cleared, the BMS must exit the AIR shutdown state and enter the init state.

## DIM <a name="DIM"></a>
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
DIM-0 | Startup CAN message | The DIM must transmit a startup message over CAN on boot.
DIM-1 | Heartbeat receiving | The DIM must set the 7-segments all on to display '888' once it does not receive three consecutive BMS heartbeats.
DIM-2 | Board status LEDs | The DIM must indicate the current status of the BMS, DCM, DIM, FSM and PDM using RGB LEDs, where GREEN = no fault, BLUE = non-critical fault and RED = critical fault. | EV.6.1.11
DIM-3 | Drive mode switch | The DIM must transmit the drive mode position of the rotarty switch over CAN at 100Hz.
DIM-4 |  Start, traction control, torque vectoring switches | For each of the switches, the DIM must: <br/> - Transmit the on/off switch status of over CAN at 100Hz. <br/> - Set the corresponding green status LEDs when the switch is on.
DIM-5 | IMD LED | The DIM must turn on the IMD LED when it receives IMD fault status from BMS over CAN. | EV.8.5.5
DIM-6 | BSPD LED | The DIM must turn on the BSPD LED when it receives BSPD fault status from FSM over CAN. 
DIM-7 | Regen reporting | The DIM must report the regen paddle percentage over CAN at 100Hz.
DIM-8 | Regen maping | The DIM must linearly map the the peddle position as a percentage (0% - fully de-pressed, 100% - fully pressed).
DIM-9 | 7-segment |  - The DIM must display the SoC as percentage on the 7-segment displays if no faults are present. <br/> - If a fault has occurred the DIM must stop displaying the SoC and instead display any faults onto the 7-segment displays. <br/><ul>- The first 7-segment must display the board ID while the remaining two must display the fault ID. <br/> <img src="https://user-images.githubusercontent.com/25499626/60911239-06cca080-a283-11e9-9dfa-62fcb814f2c1.png" width="200"> <br/>- If there are more than one fault active, the DIM must cycle through displaying each present fault at 1Hz. </ul>
