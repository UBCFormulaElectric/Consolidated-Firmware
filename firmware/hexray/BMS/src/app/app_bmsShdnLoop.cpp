#include <array>

#include "app_bmsShdnLoop.hpp"
#include "io_bmsShdn.hpp"

using namespace io::shdn;
namespace app::shdn
{
const shdnLoop bms_shdnLoop{ std::array<const node *const, 3>{ { &hv_p_ok_node, &hv_n_ok_node, &loop_ok_node } } };
}
