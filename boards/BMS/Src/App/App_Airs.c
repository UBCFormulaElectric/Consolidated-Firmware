#include "App_Airs.h"

#include <assert.h>
#include <stdlib.h>

struct Airs {
  bool (*is_air_positive_closed)(void);
  bool (*is_air_negative_closed)(void);

  void (*close_air_positive)(void);
  void (*open_air_positive)(void);
};

struct Airs *App_Airs_Create(bool (*is_air_positive_closed)(void),
                             bool (*is_air_negative_closed)(void),
                             void (*close_air_positive)(void),
                             void (*open_air_positive)(void)) {
  struct Airs *airs = malloc(sizeof(struct Airs));
  assert(airs != NULL);

  airs->is_air_positive_closed = is_air_positive_closed;
  airs->is_air_negative_closed = is_air_negative_closed;
  airs->close_air_positive = close_air_positive;
  airs->open_air_positive = open_air_positive;

  return airs;
}

void App_Airs_Destroy(struct Airs *airs) { free(airs); }

bool App_Airs_IsAirPositiveClosed(const struct Airs *const airs) {
  return airs->is_air_positive_closed();
}

bool App_Airs_IsAirNegativeClosed(const struct Airs *const airs) {
  return airs->is_air_negative_closed();
}

void App_Airs_CloseAirPositive(const struct Airs *const airs) {
  airs->close_air_positive();
}

void App_Airs_OpenAirPositive(const struct Airs *const airs) {
  airs->open_air_positive();
}
