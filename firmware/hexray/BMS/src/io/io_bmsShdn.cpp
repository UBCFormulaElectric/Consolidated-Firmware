#include "io_bmsShdn.hpp"
#include "hw_gpios.hpp"
#include "app_canTx.hpp"
namespace io::shdn
{
node hv_p_ok_node(hv_p_intlck_sns, app::can_tx::BMS_HVPShdnOKStatus_set);
node hv_n_ok_node(hv_n_intlck_sns, app::can_tx::BMS_HVNShdnOKStatus_set);
node loop_ok_node(shdn_term_sns, app::can_tx::BMS_ShdnTermOKStatus_set);
} // namespace io::shdn