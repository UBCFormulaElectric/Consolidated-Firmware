#include "App_TestWorld.h"

#include <stdlib.h>

struct TestWorld
{
};

struct TestWorld *App_TestWorld_Create(void)
{
    struct TestWorld *test_world = (struct TestWorld *)malloc(sizeof(struct TestWorld));

    return test_world;
}

void App_TestWorld_Destroy(struct TestWorld *test_world)
{
    free(test_world);
}
