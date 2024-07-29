#pragma once
#include "io_chimera.h"

#include "hw_gpios.h"
#include "hw_adcs.h"
#include "CRIT.pb.h"

const hw::Gpio *id_to_gpio(uint32_t net_name)
{
    switch ((CRIT_GpioNetName)net_name)
    {
        CASERETURN(CRIT_GpioNetName_TORQUE_VECTORING_LED, &hw::gpio::torquevec_led_pin);
        CASERETURN(CRIT_GpioNetName_START_LED, &hw::gpio::start_led_pin);
        CASERETURN(CRIT_GpioNetName_REGEN_LED, &hw::gpio::regen_led_pin);
        CASERETURN(CRIT_GpioNetName_AUX_DB_R, &hw::gpio::aux_r_pin);
        CASERETURN(CRIT_GpioNetName_CRIT_DB_R, &hw::gpio::crit_r_pin);
        CASERETURN(CRIT_GpioNetName_CRIT_DB_G, &hw::gpio::crit_g_pin);
        CASERETURN(CRIT_GpioNetName_CRIT_DB_B, &hw::gpio::crit_b_pin);
        CASERETURN(CRIT_GpioNetName_AUX_DB_G, &hw::gpio::aux_g_pin);
        CASERETURN(CRIT_GpioNetName_AUX_DB_B, &hw::gpio::aux_b_pin);
        CASERETURN(CRIT_GpioNetName_BSPD_R, &hw::gpio::bspd_r_pin);
        CASERETURN(CRIT_GpioNetName_SHDN_R, &hw::gpio::shdn_r_pin);
        CASERETURN(CRIT_GpioNetName_RSM_B, &hw::gpio::rsm_b_pin);
        CASERETURN(CRIT_GpioNetName_VC_R, &hw::gpio::rvc_r_pin);
        CASERETURN(CRIT_GpioNetName_VC_B, &hw::gpio::rvc_b_pin);
        CASERETURN(CRIT_GpioNetName_FSM_R, &hw::gpio::fvc_r_pin);
        CASERETURN(CRIT_GpioNetName_BMS_R, &hw::gpio::bms_r_pin);
        CASERETURN(CRIT_GpioNetName_BMS_G, &hw::gpio::bms_g_pin);
        CASERETURN(CRIT_GpioNetName_BMS_B, &hw::gpio::bms_b_pin);
        CASERETURN(CRIT_GpioNetName_AMS_R, &hw::gpio::ams_r_pin);
        CASERETURN(CRIT_GpioNetName_NDRIVE_MODE_1b, &hw::gpio::n_drive_mode_1_pin);
        CASERETURN(CRIT_GpioNetName_IMD_R, &hw::gpio::imd_r_pin);
        CASERETURN(CRIT_GpioNetName_INERTIA_SEN, &hw::gpio::inertia_sen_pin);
        CASERETURN(CRIT_GpioNetName_FSM_G, &hw::gpio::fvc_g_pin);
        CASERETURN(CRIT_GpioNetName_FSM_B, &hw::gpio::fvc_b_pin);
        CASERETURN(CRIT_GpioNetName_RSM_R, &hw::gpio::rsm_r_pin);
        CASERETURN(CRIT_GpioNetName_RSM_G, &hw::gpio::rsm_g_pin);
        CASERETURN(CRIT_GpioNetName_VC_G, &hw::gpio::rvc_g_pin);
        CASERETURN(CRIT_GpioNetName_SHDN_G, &hw::gpio::shdn_g_pin);
        CASERETURN(CRIT_GpioNetName_NDRIVE_MODE_2b, &hw::gpio::n_drive_mode_2_pin);
        CASERETURN(CRIT_GpioNetName_NDRIVE_MODE_3b, &hw::gpio::n_drive_mode_3_pin);
        CASERETURN(CRIT_GpioNetName_NDRIVE_MODE_0b, &hw::gpio::n_drive_mode_0_pin);
        CASERETURN(CRIT_GpioNetName_TORQUE_VECTORING_SIG, &hw::gpio::torquevec_switch_gpio);
        CASERETURN(CRIT_GpioNetName_REGEN_SIG, &hw::gpio::regen_switch_gpio);
        CASERETURN(CRIT_GpioNetName_START_SIG, &hw::gpio::start_switch_gpio);
        CASERETURN(CRIT_GpioNetName_SDHN_SEN, &hw::gpio::cockpit_estop_sen_pin);
        CASERETURN(CRIT_GpioNetName_NPROGRAM_3V3, &hw::gpio::n_program_pin);
        CASERETURN(CRIT_GpioNetName_NCHIMERA, &hw::gpio::n_chimera_pin);
        CASERETURN(CRIT_GpioNetName_LED, &hw::gpio::led_pin);
        default:
            assert(false);
    }
}

const hw::Adc *id_to_adc(uint32_t net_name)
{
    switch ((CRIT_AdcNetName)net_name)
    {
        CASERETURN(CRIT_AdcNetName_REGEN_3V3, &hw::adc::regen_3v3_adc);
        default:
            assert(false);
    }
}