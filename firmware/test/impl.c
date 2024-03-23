#include "impl.h"
#include <stdio.h>

static int count;

int impl_add(int a, int b)
{
    return a + b;
}

int impl_inc(void)
{
    return ++count;
}

void impl_print(void)
{
    printf("hello world!\n");
}