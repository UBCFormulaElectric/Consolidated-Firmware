#include "App_BinarySwitch.h"

#include <assert.h>
#include <stdlib.h>

struct BinarySwitch {
  bool (*is_turned_on)(void);
};

struct BinarySwitch *App_BinarySwitch_Create(bool (*const is_turned_on)(void)) {
  assert(is_turned_on != NULL);

  struct BinarySwitch *binary_switch = malloc(sizeof(struct BinarySwitch));
  assert(binary_switch != NULL);

  binary_switch->is_turned_on = is_turned_on;

  return binary_switch;
}

void App_BinarySwitch_Destroy(struct BinarySwitch *const binary_switch) {
  free(binary_switch);
}

bool App_BinarySwitch_IsTurnedOn(
    const struct BinarySwitch *const binary_switch) {
  return binary_switch->is_turned_on();
}
