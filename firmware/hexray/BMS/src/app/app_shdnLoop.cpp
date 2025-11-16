#include <array>
#include "app_shdnLoop.hpp"
#include "io_shdnLoopNode.hpp"

using namespace io::shdn;
namespace
{
constexpr node hv_p_ok_node(hv_p_intlck_sns_pin, app_canTx_BMS_HVPShdnOKStatus_set);
constexpr node hv_n_ok_node(hv_n_intlck_sns_pin, app_canTx_BMS_HVNShdnOKStatus_set);
} // namespace

namespace app::shdn
{
const shdnLoop<2> bms_shdnLoop{ std::array<const io::shdn::node *const, 2>{ &hv_p_ok_node, &hv_n_ok_node } };
}
