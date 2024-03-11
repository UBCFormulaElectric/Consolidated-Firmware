#include "api1.h"
#include "impl.h"

__VISIBLE int api1_add(int a, int b)
{
    return impl_add(a, b);
}

__VISIBLE int api1_inc(void)
{
    return impl_inc();
}