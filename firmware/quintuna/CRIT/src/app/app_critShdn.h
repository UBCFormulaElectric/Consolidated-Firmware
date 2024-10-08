#pragma once
#include <array>
#include "app_shdnLoop.h"
#include "io_critShdn.h"

namespace app::shdn
{
static const shdnLoop<2> critShdn{ { &io::shdn::cockpit_estop_node, &io::shdn::inertia_sen_node } };
}