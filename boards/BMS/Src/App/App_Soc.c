#include <math.h>
#include <stdint.h>
#include "App_Soc.h"
#include "App_SharedMacros.h"
#include "App_SharedProcessing.h"
#include "lut/App_Lut_CellVoltageToSoc.h"

struct StateOfCharge
{
    float initial_soc;
    float curr_soc;
    float charge_removed_in_ah;
};

static struct StateOfCharge state_of_charge = { 0U };

static inline float App_CalculateSocInPercent(float curr_soc_ah)
{
    return curr_soc_ah / soc_amp_hr_lookup[0] * 100.0f;
}

ExitCode App_Soc_Vote(float max_abs_difference, float soc_1, float soc_2, float soc_3, float *result)
{
    if (max_abs_difference < 0.0f || max_abs_difference > 100.0f || soc_1 < 0.0f || soc_1 > 100.0f || soc_2 < 0.0f ||
        soc_2 > 100.0f || soc_3 < 0.0f || soc_3 > 100.0f)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    float diff = 0.0f;

    diff = fabsf(soc_1 - soc_2);
    if (diff <= max_abs_difference)
    {
        *result = (soc_1 + soc_2) / 2.0f;
        return EXIT_CODE_OK;
    }

    diff = fabsf(soc_2 - soc_3);
    if (diff <= max_abs_difference)
    {
        *result = (soc_2 + soc_3) / 2.0f;
        return EXIT_CODE_OK;
    }

    diff = fabsf(soc_1 - soc_3);
    if (diff <= max_abs_difference)
    {
        *result = (soc_1 + soc_3) / 2.0f;
        return EXIT_CODE_OK;
    }

    *result = NAN;
    return EXIT_CODE_OK;
}

void App_Soc_GetInitialSoc(float min_cell_voltage)
{
    uint8_t v_to_soc_lut_index = 0U;
    for (v_to_soc_lut_index = 0U; min_cell_voltage < v_to_soc_lookup[v_to_soc_lut_index]; v_to_soc_lut_index++)
        ;
    state_of_charge.initial_soc = App_CalculateSocInPercent(soc_amp_hr_lookup[v_to_soc_lut_index]);
    // TODO: Store SoC in the EEPROM
}

void App_Soc_CoulombCount(float *prev_output_current, float curr_output_current, float time_interval_s)
{
    // Integrate the charge removed in Ah
    App_SharedProcessing_TrapezoidalRule(
        &state_of_charge.charge_removed_in_ah, prev_output_current, curr_output_current,
        SECONDS_TO_HOUR(time_interval_s));

    // Perform coulomb counting only in the drive state
    state_of_charge.curr_soc =
        App_CalculateSocInPercent(state_of_charge.initial_soc - state_of_charge.charge_removed_in_ah);
}

float App_Soc_GetSoc(void)
{
    // We expect to use this only when we are in the drive state
    return state_of_charge.curr_soc;
}

void App_Soc_GetSocFromEeprom(void)
{
    // TODO: Update the state of charge with the value received from the EEPROM. We expect to do this only when we are
    // entering the drive state
    state_of_charge.initial_soc = 0.0f;
}

void App_Soc_WriteSocToEeprom(void)
{
    // TODO: This function should only be used in the drive state. Write the state of charge to the EEPROM. Reset the
    // amount of charge removed from the pack
}
