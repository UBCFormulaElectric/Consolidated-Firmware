#include "App_GsmWorld.h"

#include <assert.h>
#include <stdlib.h>

struct GsmWorld {
  struct Clock *clock;
};

struct GsmWorld *App_GsmWorld_Create(struct Clock *clock) {
  struct GsmWorld *world = (struct GsmWorld *)malloc(sizeof(struct GsmWorld));
  assert(world != NULL);

  world->clock = clock;

  return world;
}

void App_GsmWorld_Destroy(struct GsmWorld *world) { free(world); }
