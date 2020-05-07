#pragma once

struct TestWorld;

/**
 * Create a TestWorld
 * @return A created TestWorld
 */
struct TestWorld* App_TestWorld_Create(void);

/**
 * Deallocate the given test world
 *
 * @param test_world
 */
void App_TestWorld_Destroy(struct TestWorld* test_world);
