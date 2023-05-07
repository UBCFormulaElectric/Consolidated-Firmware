#include "App_DimWorld.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

struct DimWorld {
  struct SevenSegDisplays *seven_seg_displays;
  struct HeartbeatMonitor *heartbeat_monitor;
  struct RgbLedSequence *rgb_led_sequence;
  struct RotarySwitch *drive_mode_switch;
  struct Led *imd_led;
  struct Led *bspd_led;
  struct BinarySwitch *start_switch;
  struct BinarySwitch *aux_switch;
  struct RgbLed *bms_status_led;
  struct RgbLed *dcm_status_led;
  struct RgbLed *dim_status_led;
  struct RgbLed *fsm_status_led;
  struct RgbLed *pdm_status_led;
  struct Clock *clock;
};

struct DimWorld *App_DimWorld_Create(
    struct SevenSegDisplays *const seven_seg_displays,
    struct HeartbeatMonitor *const heartbeat_monitor,
    struct RgbLedSequence *const rgb_led_sequence,
    struct RotarySwitch *const drive_mode_switch, struct Led *const imd_led,
    struct Led *const bspd_led, struct BinarySwitch *const start_switch,
    struct BinarySwitch *const aux_switch, struct RgbLed *const bms_status_led,
    struct RgbLed *const dcm_status_led, struct RgbLed *const dim_status_led,
    struct RgbLed *const fsm_status_led, struct RgbLed *const pdm_status_led,
    struct Clock *const clock) {
  struct DimWorld *world = (struct DimWorld *)malloc(sizeof(struct DimWorld));
  assert(world != NULL);

  world->seven_seg_displays = seven_seg_displays;
  world->heartbeat_monitor = heartbeat_monitor;
  world->rgb_led_sequence = rgb_led_sequence;
  world->drive_mode_switch = drive_mode_switch;
  world->imd_led = imd_led;
  world->bspd_led = bspd_led;
  world->start_switch = start_switch;
  world->aux_switch = aux_switch;
  world->bms_status_led = bms_status_led;
  world->dcm_status_led = dcm_status_led;
  world->dim_status_led = dim_status_led;
  world->fsm_status_led = fsm_status_led;
  world->pdm_status_led = pdm_status_led;
  world->clock = clock;

  return world;
}

void App_DimWorld_Destroy(struct DimWorld *world) { free(world); }

struct SevenSegDisplays *App_DimWorld_GetSevenSegDisplays(
    const struct DimWorld *const world) {
  return world->seven_seg_displays;
}

struct HeartbeatMonitor *App_DimWorld_GetHeartbeatMonitor(
    const struct DimWorld *const world) {
  return world->heartbeat_monitor;
}

struct RgbLedSequence *App_DimWorld_GetRgbLedSequence(
    const struct DimWorld *const world) {
  return world->rgb_led_sequence;
}

struct RotarySwitch *App_DimWorld_GetDriveModeSwitch(
    const struct DimWorld *const world) {
  return world->drive_mode_switch;
}

struct Led *App_DimWorld_GetImdLed(const struct DimWorld *const world) {
  return world->imd_led;
}

struct Led *App_DimWorld_GetBspdLed(const struct DimWorld *world) {
  return world->bspd_led;
}

struct BinarySwitch *App_DimWorld_GetStartSwitch(
    const struct DimWorld *const world) {
  return world->start_switch;
}

struct BinarySwitch *App_DimWorld_GetAuxSwitch(
    const struct DimWorld *const world) {
  return world->aux_switch;
}

struct RgbLed *App_DimWorld_GetBmsStatusLed(const struct DimWorld *world) {
  return world->bms_status_led;
}

struct RgbLed *App_DimWorld_GetDcmStatusLed(const struct DimWorld *world) {
  return world->dcm_status_led;
}

struct RgbLed *App_DimWorld_GetDimStatusLed(const struct DimWorld *world) {
  return world->dim_status_led;
}

struct RgbLed *App_DimWorld_GetFsmStatusLed(const struct DimWorld *world) {
  return world->fsm_status_led;
}

struct RgbLed *App_DimWorld_GetPdmStatusLed(const struct DimWorld *world) {
  return world->pdm_status_led;
}

struct Clock *App_DimWorld_GetClock(const struct DimWorld *world) {
  return world->clock;
}
