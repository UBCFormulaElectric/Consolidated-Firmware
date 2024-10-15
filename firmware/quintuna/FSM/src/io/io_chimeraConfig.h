#pragma once
#include "io_chimera.h"

#include "hw_gpios.h"
#include "hw_adcs.h"
#include "FSM.pb.h"

const hw::Gpio *id_to_gpio(uint32_t net_name)
{
    switch (static_cast<FSM_GpioNetName>(net_name))
    {
        CASERETURN(FSM_GpioNetName_BRAKE_OCSC_OK_3V3, &hw::gpio::brake_ocsc_ok_3v3);
        CASERETURN(FSM_GpioNetName_NCHIMERA, &hw::gpio::n_chimera_pin);
        CASERETURN(FSM_GpioNetName_LED, &hw::gpio::led);
        CASERETURN(FSM_GpioNetName_NBSPD_BRAKE_PRESSED_3V3, &hw::gpio::nbspsd_brake_pressed_3v3);
        CASERETURN(FSM_GpioNetName_NPROGRAM_3V3, &hw::gpio::n_program_3v3);
        CASERETURN(FSM_GpioNetName_FSM_SHDN, &hw::gpio::fsm_shdn);

        default:
            assert(false);
    }
}

const hw::Adc *id_to_adc(uint32_t net_name)
{
    switch (static_cast<FSM_AdcNetName>(net_name))
    {
        CASERETURN(FSM_AdcNetName_LOAD_CELL_1_3V3, &hw::adc::load_cell_1_adc);
        CASERETURN(FSM_AdcNetName_APPS1_3V3, &hw::adc::apps1_adc);
        CASERETURN(FSM_AdcNetName_APPS2_3V3, &hw::adc::apps2_adc);
        CASERETURN(FSM_AdcNetName_BPS_B_3V3, &hw::adc::break_pres_sensor_f_adc);
        CASERETURN(FSM_AdcNetName_BPS_F_3V3, &hw::adc::break_pres_sensor_b_adc);
        CASERETURN(FSM_AdcNetName_LOAD_CELL_2_3V3, &hw::adc::load_cell_2_adc);
        CASERETURN(FSM_AdcNetName_SteeringAngle_3V3, &hw::adc::steering_angle_adc);
        CASERETURN(FSM_AdcNetName_SUSP_TRAVEL_FL_3V3, &hw::adc::susp_travel_fl_adc);
        CASERETURN(FSM_AdcNetName_SUSP_TRAVEL_FR_3V3, &hw::adc::susp_travel_fr_adc);
        
        default:
            assert(false);
    }
}