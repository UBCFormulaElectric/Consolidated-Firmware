#include "app_utils.h"
#include "io_efuse_ST_VND5.h"
#include "io_efuse/io_efuse_datatypes.h"
#include "hw_gpio.h"
#include <stdbool.h>
#include <assert.h>

/**
 * ST_VND5 Efuse Datasheet:
 * https://octopart.com/datasheet/vnd5t100ajtr-e-stmicroelectronics-21218840
 */

#define NOMINAL_V 3.0f
// Vsenseh lower threshold
#define V_SENSE_H_L 7.5f
// Vsenseh upper threshold
#define V_SENSE_H_H 9.5f

#define V_THRES 0.1f

#define ST_VND5_Efuse_FAULT_FLAGS 0x3F

// {fault_reset_stby, channel_enabled}
#define L_L 0x00U
#define L_H 0x01U
#define H_L 0x02U
#define H_H 0x03U

static void  io_efuse_ST_setChannel(const Efuse *channel, bool enabled);
static bool  io_efuse_ST_isChannelEnabled(const Efuse *channel);
static float io_efuse_ST_getChannelCurrent(const Efuse *channel);
static void  io_efuse_ST_reset_set(const Efuse *channel, bool set);
static void  io_efuse_ST_reset(const Efuse *channel);
static bool  io_efuse_ST_ok(const Efuse *efuse);

const EfuseFunctions ST_VND5_Efuse_functions = { .set_channel          = io_efuse_ST_setChannel,
                                                 .is_channel_enabled   = io_efuse_ST_isChannelEnabled,
                                                 .get_channel_current  = io_efuse_ST_getChannelCurrent,
                                                 .loadswitch_reset_set = io_efuse_ST_reset_set,
                                                 .reset_efuse          = io_efuse_ST_reset,
                                                 .pgood                = NULL,
                                                 .efuse_ok             = io_efuse_ST_ok };

static void io_efuse_ST_setChannel(const Efuse *channel, bool enabled)
{
    assert(channel->enable_gpio != NULL);
    hw_gpio_writePin(channel->enable_gpio, enabled);
}

static bool io_efuse_ST_isChannelEnabled(const Efuse *channel)
{
    assert(channel->enable_gpio != NULL);
    return hw_gpio_readPin(channel->enable_gpio);
}

static float io_efuse_ST_getChannelCurrent(const Efuse *channel)
{
    const AdcChannel *current_sense = channel->sns_adc_channel;
    assert(current_sense != NULL);
    return hw_adc_getVoltage(current_sense) * ADC_VOLTAGE_TO_CURRENT_A;
}

static void io_efuse_ST_reset_set(const Efuse *channel, bool set)
{
    assert(channel->st_vnd5->stby_reset_gpio != NULL);
    hw_gpio_writePin(channel->st_vnd5->stby_reset_gpio, set);
}

static void io_efuse_ST_reset(const Efuse *channel)
{
    assert(channel->st_vnd5->stby_reset_gpio != NULL);
    hw_gpio_writePin(channel->st_vnd5->stby_reset_gpio, false);
    hw_gpio_writePin(channel->st_vnd5->stby_reset_gpio, true);
    hw_gpio_writePin(channel->st_vnd5->stby_reset_gpio, false);
}

static bool io_efuse_ST_ok(const Efuse *efuse)
{
    assert(efuse != NULL);

    float   voltage          = io_efuse_ST_getChannelCurrent(efuse) / ADC_VOLTAGE_TO_CURRENT_A;
    bool    channelEnabled   = io_efuse_ST_isChannelEnabled(efuse);
    bool    fault_reset_stby = hw_gpio_readPin(efuse->st_vnd5->stby_reset_gpio);
    uint8_t fault_table_idx  = 0x00U | ((uint8_t)((fault_reset_stby << 1) | (channelEnabled)));

    // Setting faults for st_vnd5 efuse
    // TODO: Verify these
    if (fault_table_idx == L_L)
    {
        efuse->st_vnd5->faults.flags.overload                      = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
        efuse->st_vnd5->faults.flags.ovt_stp                       = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
        efuse->st_vnd5->faults.flags.short_to_vbat                 = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
        efuse->st_vnd5->faults.flags.open_load_off_stat            = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
        efuse->st_vnd5->faults.flags.negative_output_voltage_clamp = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
    }
    else if (fault_table_idx == L_H)
    {
        efuse->st_vnd5->faults.flags.overload           = APPROX_EQUAL_FLOAT(voltage, NOMINAL_V, V_THRES);
        efuse->st_vnd5->faults.flags.ovt_stp            = IS_IN_RANGE(V_SENSE_H_L, V_SENSE_H_H, voltage);
        efuse->st_vnd5->faults.flags.short_to_vbat      = APPROX_EQUAL_FLOAT(voltage, NOMINAL_V, V_THRES);
        efuse->st_vnd5->faults.flags.open_load_off_stat = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
        // efuse->st_vnd5->faults.flags.negative_output_voltage_clamp = (voltage <= 0.0f);
    }
    else if (fault_table_idx == H_L)
    {
        efuse->st_vnd5->faults.flags.overload                      = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
        efuse->st_vnd5->faults.flags.ovt_stp                       = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
        efuse->st_vnd5->faults.flags.short_to_vbat                 = IS_IN_RANGE(V_SENSE_H_L, V_SENSE_H_H, voltage);
        efuse->st_vnd5->faults.flags.open_load_off_stat            = IS_IN_RANGE(V_SENSE_H_L, V_SENSE_H_H, voltage);
        efuse->st_vnd5->faults.flags.negative_output_voltage_clamp = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
    }
    else if (fault_table_idx == H_H)
    {
        efuse->st_vnd5->faults.flags.overload           = APPROX_EQUAL_FLOAT(voltage, NOMINAL_V, V_THRES);
        efuse->st_vnd5->faults.flags.ovt_stp            = IS_IN_RANGE(V_SENSE_H_L, V_SENSE_H_H, voltage);
        efuse->st_vnd5->faults.flags.short_to_vbat      = APPROX_EQUAL_FLOAT(voltage, NOMINAL_V, V_THRES);
        efuse->st_vnd5->faults.flags.open_load_off_stat = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
        // efuse->st_vnd5->faults.flags.negative_output_voltage_clamp = (voltage <= 0.0f);
    }

    efuse->st_vnd5->faults.flags.under_voltage = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);

    // check if any flag is set, then return the status
    uint8_t flags = efuse->st_vnd5->faults.raw & ST_VND5_Efuse_FAULT_FLAGS;

    // TODO: Do we want to return a boolean or should we just return the faults union and add additional methods to check specific faults
    return !(flags > 0);
}
