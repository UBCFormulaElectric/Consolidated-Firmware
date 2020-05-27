#include <assert.h>
#include <stdlib.h>
#include "App_Buzzer.h"

struct Buzzer
{
    void (*on)(void);
    void (*off)(void);
};

struct Buzzer *App_Buzzer_Create(void (*on)(void), void (*off)(void))
{
    struct Buzzer *buzzer = malloc(sizeof(struct Buzzer));
    assert(buzzer != NULL);

    buzzer->on  = on;
    buzzer->off = off;

    return buzzer;
}

void App_Buzzer_Destroy(struct Buzzer *buzzer)
{
    free(buzzer);
}

void App_Buzzer_On(const struct Buzzer *buzzer)
{
    buzzer->on();
}

void App_Buzzer_Off(const struct Buzzer *buzzer)
{
    buzzer->off();
}
