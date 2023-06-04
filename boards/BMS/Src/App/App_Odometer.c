#include "App_Odometer.h"
#include "App_CanUtils.h"
#include "App_SharedMacros.h"
#include "App_CanRx.h"

#define TIME_STEP = 0.01f
#define SECONDS_BETWEEN_ODOMETER_WRITES 30

struct Odometer
{
    float   distance_travelled_km;
    uint8_t odometer_counter;
};

struct Odometer *App_Odometer_Create(void)
{
    struct Odometer *odometer = malloc(sizeof(struct Odometer));
    assert(odometer != NULL);
    odometer->odometer_counter      = 0;
    odometer->distance_travelled_km = 0;

    return odometer;
}

void App_Odometer_Destroy(struct Odometer *odometer)
{
    free(odometer);
}

float App_Odometer_ReadValFromEeprom(struct Eeprom *eeprom, uint16_t address)
{
    return App_Eeprom_ReadErrCheckedFloat(eeprom, address);
}

ExitCode App_Odometer_WriteValToEeprom(struct Odometer *odometer, struct Eeprom *eeprom, uint16_t address)
{
    if (App_Eeprom_WriteErrCheckedFloat(eeprom, address, odometer->distance_travelled_km) != EEPROM_OK)
    {
        return EXIT_CODE_ERROR;
    }
    else
    {
        return EXIT_CODE_OK;
    }
}

float App_Odometer_UpdateReading(struct Odometer *odometer)
{
    const float avg_wheelspeed = ((float)App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Get() +
                                  (float)App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Get()) /
                                 2.0f;

    const float delta_distance_travelled = avg_wheelspeed * TIRE_CIRCUMFRANCE_KM;

    // Only increase odometer value if distance travelled is forwards
    if (delta_distance_travelled > 0.0f)
    {
        odometer->distance_travelled_km += delta_distance_travelled;
    }

    return odometer->distance_travelled_km;
}

float App_Odometer_GetReading(struct Odometer *odometer)
{
    return odometer->distance_travelled_km;
}

void App_Odometer_SetReading(struct Odometer *odometer, float odometer_value)
{
    odometer->distance_travelled_km = odometer_value;
}

bool App_Odometer_TickCounter(struct Odometer *odometer)
{
    if (odometer->odometer_counter >= SECONDS_BETWEEN_ODOMETER_WRITES)
    {
        odometer->odometer_counter = 0;
        return true;
    }
    else
    {
        odometer->odometer_counter++;
        return false;
    }
}

void App_Odometer_ResetCounter(struct Odometer *odometer)
{
    odometer->odometer_counter = 0;
}