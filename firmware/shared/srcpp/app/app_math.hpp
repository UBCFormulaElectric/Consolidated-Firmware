#pragma once
#include <cmath>

#define APPROX_EQUAL_FLOAT(a, b, threshold) ((bool)(fabsf((a) - (b)) < threshold))
#define IS_IN_RANGE(min, max, val) (((val) > (min)) && ((val) < (max)))
#define CLAMP(x, min, max) std::clamp(x, min, max);
