#pragma once

//-------------- Inverter Specs -----------------

#define MAX_INVERTER_CURRENT \
    10.0f // 148.5f // Maximum inverter current (peak), in A
#define MAX_CUR_SNS_OFFSET \
    2.5f // Maximum offset of the Allegro ACS758 200A current sensor at 0A input
#define MIN_BUS_VOLTAGE 269 // Minimum DC bus voltage, in V
#define MAX_BUS_VOLTAGE 400 // Maximum DC bus voltage, in V
#define MAX_PHASE_VOLTAGE \
    207.8460969082653          // Maximum inverter ac voltage (phase peak)
#define REGEN_POWER_LIMIT 7500 // Regen power limit, in watts
#define DEAD_TIME 1000         // Dead time for power stage switches, in ns
#define MIN_DEAD_TIME 50       // Minimum dead time for power stage, in ns
#define MAX_DEAD_TIME 2000     // Maximum dead time for power stage, in ns

//-------------- Motor Specs -----------------

// AMK DD5-14-10-POW
#define MAX_STATOR_CURRENT 85.7f // Maximum stator current for 1 second, in A
#define BACK_EMF_CONST 26.58721497261419f // Back EMF constant Vpk/krpm
#define TORQUE_CONST 0.2334286204325686f  // Torque constant, in N*m/A
#define MOTOR_POLES 10                    // Number of poles
#define STATOR_RES 0.0675f                // Stator winding resistance, in Ohm
#define D_INDUCTANCE 0.00012f             // d-axis inductance, in H
#define Q_INDUCTANCE 0.00024f             // q-axis inductance, in H
//#define SHAFT_J 0.000274                	// Moment of inertia, in kg-m^2
//#define SHAFT_TIME_CONSTANT 1000         	// Shaft time constant, in sec.
#define MAX_MOTOR_TORQUE 23.0f // Maximum motor torque, in N*m
#define MAX_MOTOR_POWER 30000  // Maximum motor power, in W
#define MAX_MOTOR_SPEED 15000  // Maximum motor speed, in rpm
#define SPEED_SETPOINT 15000   // Speed setpoint, in rpm

//-------------- Control Loop Parameters -----------------

#define SWITCHING_FREQUENCY 10000 // Switching frequency of the power stage
#define MAX_SWITCHING_FREQUENCY \
    12000 // Maximum settable switching frequency, in Hz
#define MIN_SWITCHING_FREQUENCY \
    4000                       // Minimum settable switching frequency, in Hz
#define SAMPLE_FREQUENCY 20000 // Sampling frequency of sensors for control loop
#define D_GAIN 0.8706229594480495f * 5     // PI controller D axis gain
#define D_TIME_CONST 0.001777777777777778f // PI controller D axis time constant
#define Q_GAIN 1.741245918896099f * 5      // PI controller Q axis gain
#define Q_TIME_CONST 0.003555555555555555f // PI controller Q axis time constant
#define SPEED_GAIN 0.4002385659999999f     // Speed PI controller gain
#define SPEED_TIME_CONST 0.04244135400651f // Speed PI controller time constant
#define DECOUPLING_ENABLED 0     // Enable flag for D/Q axis decoupling
#define LUT_CONTROL_ENABLED 1    // Enable flag for Motor Control with LUT
#define CALCULATE_PHC_CURRENT 1  // Enable flag for calculation of PHC current
#define ADAPTION_GAIN_ENABLED 0  // Enable flag for Adaptive gain
#define ADAPTION_GAIN_CONST 0.2f // Adaption constant for Q axis gain
#define ADAPTION_TIME_CONST 4    // Adaption constant for Q axis time constant
#define LOWER_ADAPTION_LIMIT \
    0.19f // Lower adaption max current percentage limit
#define UPPER_ADAPTION_LIMIT \
    0.68f                  // Upper adaption max current percentage limit
#define MAX_MOD_INDEX 0.9f // Maximum modulation index
#define MAX_MOTOR_POS_CHANGE_PER_CYCLE \
    (MAX_MOTOR_SPEED / 9.459f / (float)SAMPLE_FREQUENCY)
#define RMS_CALC_MIN_FREQUENCY 1 // Minimum frequency RMS calculation can handle
#define RMS_CALC_ARRAY_SIZE // Number of samples to take for RMS calculation

#define PWM_CLOCK_FREQUENCY \
    216 // Frequency of clock used for PWM edge timer, in MHz
