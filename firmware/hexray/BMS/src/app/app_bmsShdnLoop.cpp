#include <array>

#include "app_bmsShdnLoop.hpp"
#include "io_bmsShdn.hpp"

using namespace io::shdn;
namespace app::shdn
{
const shdnLoop<2> bms_shdnLoop{ std::array<const io::shdn::node *const, 2>{ { &hv_p_ok_node, &hv_n_ok_node } } };
}
