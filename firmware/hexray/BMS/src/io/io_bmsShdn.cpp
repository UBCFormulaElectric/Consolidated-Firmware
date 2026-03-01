#include "io_bmsShdn.hpp"
#include "hw_gpios.hpp"

namespace io::shdn
{

bool msd_shdn_sns_pin_get(void)
{
    return msd_shdn_sns.readPin();
}

bool hv_p_intlck_sns_pin_get(void)
{
    return hv_p_intlck_sns.readPin();
}

bool hv_n_intlck_sns_pin_get(void)
{
    return hv_n_intlck_sns.readPin();
}

} // namespace io::shdn