#pragma once

template <typename T>
void TearDownObject(T *&object, void (*destroy_func)(T *object))
{
ASSERT_TRUE(object != NULL);
destroy_func(object);
object = NULL;
}
