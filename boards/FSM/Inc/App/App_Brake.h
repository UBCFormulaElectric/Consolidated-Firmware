#include <stdbool.h>

struct Brake;

struct Brake *App_Brake_Create(bool (*is_brake_actuated)(void));

void App_Brake_Destroy(struct Brake *brake);

bool App_Brake_IsBrakeActuated(const struct Brake *const brake);
