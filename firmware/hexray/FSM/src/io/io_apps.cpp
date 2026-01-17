#include "io_apps.hpp"
// #include "hw_adcs.hpp"
#include <tgmath.h>
#include "app_utils.h"

// Just some dummy functions to pass the Github build checks, NEEDED TO IMPLEMENT THESE FUNCTIONS LATER
namespace io::apps
{
void init() {}

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
