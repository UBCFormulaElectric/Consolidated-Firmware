#include "io_apps.hpp"
// #include "hw_adcs.hpp"
#include <tgmath.h>
#include "app_utils.h"
namespace io::apps
{
static float calcAppsAngle(const float cos_law_coefficient, const float pot_len, const float cos_law_denominator)
{
    return NULL;
}

void init()
{

}

float getPrimary(void)
{
    return 0;
}

bool isPrimaryOCSC(void)
{
    return false;
}

float getSecondary(void)
{
    return 0;
}

bool isSecondaryOCSC(void)
{
    return false;
}
} // namespace io::apps
