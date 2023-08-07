#pragma once

template <typename T> void TearDownObject(T *&object, void (*destroy_func)(T *object))
{
    ASSERT_TRUE(object != NULL);
    destroy_func(object);
    object = NULL;
}

template <typename T> __inline__ T HZ_TO_MS(T hz)
{
    return (T)1000 / hz;
}
