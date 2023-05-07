#include "App_BuzzerSignals.h"

#include "App_DcmWorld.h"

bool App_BuzzerSignals_IsOn(struct DcmWorld *world) {
  struct Buzzer *buzzer = App_DcmWorld_GetBuzzer(world);
  return App_Buzzer_IsOn(buzzer);
}

void App_BuzzerSignals_Callback(struct DcmWorld *world) {
  struct Buzzer *buzzer = App_DcmWorld_GetBuzzer(world);
  App_Buzzer_TurnOff(buzzer);
}
