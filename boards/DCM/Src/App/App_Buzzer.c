#include <assert.h>
#include <stdlib.h>
#include "App_Buzzer.h"

struct Buzzer
{
    void (*turn_on)(void);
    void (*turn_off)(void);
};

struct Buzzer *App_Buzzer_Create(void (*turn_on)(void), void (*turn_off)(void))
{
    struct Buzzer *buzzer = malloc(sizeof(struct Buzzer));
    assert(buzzer != NULL);

    buzzer->turn_on  = turn_on;
    buzzer->turn_off = turn_off;

    return buzzer;
}

void App_Buzzer_Destroy(struct Buzzer *buzzer)
{
    free(buzzer);
}

void App_Buzzer_TurnOn(const struct Buzzer *buzzer)
{
    buzzer->turn_on();
}

void App_Buzzer_TurnOff(const struct Buzzer *buzzer)
{
    buzzer->turn_off();
}
