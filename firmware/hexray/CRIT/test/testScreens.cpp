#include "critTestBase.hpp"
#include "app_canRx.hpp"
#include "critMocks.hpp"

#include <iterator>

class TestScreens : public CRITBaseTest
{
};

TEST_F(TestScreens, inv)
{
    app::can_rx::INVFL_ErrorInfo_update(0);
    app::can_rx::INVFR_ErrorInfo_update(0);
    app::can_rx::INVRL_ErrorInfo_update(0);
    app::can_rx::INVRR_ErrorInfo_update(0);
    LetTimePass(100);

    // constexpr std::array<char, io::seven_seg::DIGITS> e1 = { '.', '.', '.', '.', '.', '.' };
    constexpr char e1[io::seven_seg::DIGITS] = "........";
    ASSERT_TRUE(strcmp(fakes::seven_seg::getData().data(), e1));

    app::can_rx::INVFL_ErrorInfo_update(6969);
    LetTimePass(100);

    constexpr char e2[io::seven_seg::DIGITS] = ".fl6969.";
    ASSERT_TRUE(strcmp(fakes::seven_seg::getData().data(), e2));

    app::can_rx::INVFL_ErrorInfo_update(0);
    LetTimePass(100);

    ASSERT_TRUE(strcmp(fakes::seven_seg::getData().data(), e2)); // still the same, latched

    app::can_rx::INVRL_ErrorInfo_update(1532);
    LetTimePass(100);

    constexpr char e3[io::seven_seg::DIGITS] = ".rl1532.";
    ASSERT_TRUE(strcmp(fakes::seven_seg::getData().data(), e3));
}