#pragma once
#include "ecuTestBase.hpp"
#include "jobs.hpp"

class DAMBaseTest : public EcuTestBase
{
    void board_setup() override { jobs_init(); }
    void board_teardown() override {}
};