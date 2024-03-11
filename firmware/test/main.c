#include <stdio.h>
#include "api1.h"
#include "api2.h"
#include "impl.h"

int main(void)
{
    printf("hello world!\n");
    printf("api1 add: %d\n", api1_add(1, 2));
    printf("api2 add: %d\n", api2_add(1, 2));
    printf("api1 inc: %d\n", api1_inc());
    printf("api2 inc: %d\n", api2_inc());
    printf("api2 inc: %d\n", api2_inc());
    printf("api2 inc: %d\n", api2_inc());
    printf("api2 inc: %d\n", api2_inc());
    printf("api1 inc: %d\n", api1_inc());

    // impl_inc();
    // impl_inc();
    // impl_inc();
    // impl_inc();
    // impl_inc();
    // impl_inc();
    // impl_inc();
    // impl_inc();
    // impl_inc();
    printf("api2 inc: %d\n", api2_inc());
    printf("api1 inc: %d\n", api1_inc());

    // impl_print();

    return 0;
}