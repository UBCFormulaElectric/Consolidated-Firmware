#include <gtest/gtest.h>
#include "io_log.hpp"

class TestLog : public testing::Test
{
};

TEST_F(TestLog, print_info)
{
    LOG_INFO("Hello, %s!", "world");
}
TEST_F(TestLog, print_warning)
{
    LOG_WARN("Hello, %s!", "world");
}
TEST_F(TestLog, print_error)
{
    LOG_ERROR("Hello, %s!", "world");
}
