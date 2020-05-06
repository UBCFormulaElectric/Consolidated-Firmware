#pragma once

#include <fff.h>
#include <gtest/gtest.h>

template <typename T>
void TearDownObject(T *&object, void (*destroy_func)(T *object))
{
    ASSERT_TRUE(object != NULL);
    destroy_func(object);
    object = NULL;
}
