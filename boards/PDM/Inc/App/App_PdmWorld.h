#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "App_PdmWorld.h"

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_InRangeCheck.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedRgbLedSequence.h"
#include "App_LowVoltageBattery.h"
#include "App_Efuse.h"
#include "App_RailMonitoring.h"
#include "App_LoadSwitch.h"
#include "App_SharedClock.h"

struct PdmWorld;

struct PdmWorld *App_PdmWorld_Create(
    struct PdmCanTxInterface *const can_tx_interface,
    struct PdmCanRxInterface *const can_rx_interface,
    struct HeartbeatMonitor *const  heartbeat_monitor,
    struct RgbLedSequence *const    rgb_led_sequence,
    struct LowVoltageBattery *const low_voltage_battery,
    struct Efuse *const             efuse1,
    struct Efuse *const             efuse2,
    struct Efuse *const             efuse3,
    struct Efuse *const             efuse4,
    struct RailMonitoring *const    rail_monitor,
    bool *                          rail_range_check,
    struct Clock *const             clock);

void App_PdmWorld_Destroy(struct PdmWorld *const world);

struct PdmCanTxInterface *App_PdmWorld_GetCanTx(const struct PdmWorld *const world);

struct PdmCanRxInterface *App_PdmWorld_GetCanRx(const struct PdmWorld *const world);
struct HeartbeatMonitor * App_PdmWorld_GetHeartbeatMonitor(const struct PdmWorld *const world);

struct RgbLedSequence *   App_PdmWorld_GetRgbLedSequence(const struct PdmWorld *const world);
struct LowVoltageBattery *App_PdmWorld_GetLowVoltageBattery(const struct PdmWorld *const world);

struct Efuse *App_PdmWorld_GetEfuse1(const struct PdmWorld *const world);

struct Efuse *App_PdmWorld_GetEfuse2(const struct PdmWorld *const world);

struct Efuse *App_PdmWorld_GetEfuse3(const struct PdmWorld *const world);

struct Efuse *App_PdmWorld_GetEfuse4(const struct PdmWorld *const world);

struct RailMonitoring *App_PdmWorld_GetRailMonitoring(const struct PdmWorld *const world);

struct Clock *App_PdmWorld_GetClock(const struct PdmWorld *const world);
