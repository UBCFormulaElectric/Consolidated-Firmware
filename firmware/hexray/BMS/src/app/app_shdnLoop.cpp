#include <array>
#include "app_shdnLoop.hpp"
#include "app_canTx.hpp"
#include "hw_gpios.hpp"

#ifdef TARGET_EMBEDDED
#include "io_shdnLoopNode.hpp"
#elif TARGET_TEST
#include "io_shdnLoopNodeFake.hpp"
#endif

using namespace io::shdn;
namespace
{
#ifdef TARGET_EMBEDDED
const node hv_p_ok_node(hv_p_intlck_sns, app::can_tx::BMS_HVPShdnOKStatus_set);
const node hv_n_ok_node(hv_n_intlck_sns, app::can_tx::BMS_HVNShdnOKStatus_set);
#elif TARGET_TEST
node hv_p_ok_node(true, app_canTx_BMS_HVPShdnOKStatus_set);
node hv_n_ok_node(true, app_canTx_BMS_HVNShdnOKStatus_set);
#endif
} // namespace

namespace app::shdn
{
const shdnLoop<2> bms_shdnLoop{ std::array<const io::shdn::node *const, 2>{ { &hv_p_ok_node, &hv_n_ok_node } } };
}
