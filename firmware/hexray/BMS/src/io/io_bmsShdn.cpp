#include "io_bmsShdn.hpp"
#include "hw_gpios.hpp"
namespace io::shdn
{
node hv_p_ok_node(hv_p_intlck_sns, app::can_tx::BMS_HVPShdnOKStatus_set);
node hv_n_ok_node(hv_n_intlck_sns, app::can_tx::BMS_HVNShdnOKStatus_set);
} // namespace io::shdn