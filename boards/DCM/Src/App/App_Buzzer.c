#include "App_Buzzer.h"

#include <assert.h>
#include <stdlib.h>

struct Buzzer {
  bool is_on;
  void (*turn_on)(void);
  void (*turn_off)(void);
};

struct Buzzer *App_Buzzer_Create(void (*turn_on)(void),
                                 void (*turn_off)(void)) {
  struct Buzzer *buzzer = malloc(sizeof(struct Buzzer));
  assert(buzzer != NULL);

  buzzer->is_on = false;
  buzzer->turn_on = turn_on;
  buzzer->turn_off = turn_off;

  return buzzer;
}

void App_Buzzer_Destroy(struct Buzzer *buzzer) { free(buzzer); }

void App_Buzzer_TurnOn(struct Buzzer *buzzer) {
  buzzer->is_on = true;
  buzzer->turn_on();
}

void App_Buzzer_TurnOff(struct Buzzer *buzzer) {
  buzzer->is_on = false;
  buzzer->turn_off();
}

bool App_Buzzer_IsOn(struct Buzzer *buzzer) { return buzzer->is_on; }
