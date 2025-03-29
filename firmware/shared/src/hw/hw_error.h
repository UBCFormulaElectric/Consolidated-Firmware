#pragma once

// This is a "hack" taken from the ST forum to do the following:
//
// - Replace "Error_Handler(void)" with "Error_Handler(void)" (no effect)
// - Replace calls to "Error_Handler()" with "hw_error()"
//
// This invokes our custom assert function which will let us know the line number of the failure.
#define GET_MACRO(_0, _1, NAME, ...) NAME
#define Error_Handler(...) GET_MACRO(_0, ##__VA_ARGS__, Error_Handler1, Error_Handler0)()
#define Error_Handler0() hw_error(__FILE__, __LINE__, __func__)
#define Error_Handler1(unused) Error_Handler(unused)

void hw_error(const char *file, int line, const char *func);