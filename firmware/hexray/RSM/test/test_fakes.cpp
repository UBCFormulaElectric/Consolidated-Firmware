#include "test_fakes.hpp"
#include "io_brake.hpp"
#include "io_coolant.hpp"
#include "io_suspension.hpp"
#include "io_tireTemp.hpp"
#include "io_imus.hpp"
#include "io_rsmShdn.hpp"
#include "app_canTx.hpp"
#include "io_canQueues.hpp"

namespace fakes::io
{
namespace brake
{
    static float rearPressurePsi = 0.0f;

    void setRearPressurePsi(const float value)
    {
        rearPressurePsi = value;
    }
} // namespace brake

namespace coolant
{
    static float flowRate = 0.0f;
    void         setFlowRate(float value)
    {
        flowRate = value;
    }
} // namespace coolant

namespace suspension
{
    static float RLTravel = 0.0f;
    static float RRTravel = 0.0f;
    static bool  RROCSC   = false;
    static bool  RLOCSC   = false;

    void setRLTravel(float value)
    {
        RLTravel = value;
    }

    void setRRTravel(float value)
    {
        RRTravel = value;
    }

    void setRR_OCSC(bool value)
    {
        RROCSC = value;
    }

    void setRL_OCSC(bool value)
    {
        RLOCSC = value;
    }
} // namespace suspension

namespace rPump
{
    static uint8_t percentage = 0;

    void set_readPercentage(const uint8_t value)
    {
        percentage = value;
    }
} // namespace rPump
namespace tireTemp
{
    static float temperature = 0.0f;

    void set(const float value)
    {
        temperature = value;
    }
} // namespace tireTemp
} // namespace fakes::io

namespace io
{
namespace imus
{
    Imu imu_rear;

    std::expected<void, ErrorCode> init()
    {
        return imu_rear.init();
    }
} // namespace imus
namespace brake
{
    float getRearPressurePsi()
    {
        return fakes::io::brake::rearPressurePsi;
    }
} // namespace brake

namespace coolant
{
    float getFlowRate()
    {
        return fakes::io::coolant::flowRate;
    }
} // namespace coolant

namespace suspension
{
    float getRLTravel()
    {
        return fakes::io::suspension::RLTravel;
    }

    float getRRTravel()
    {
        return fakes::io::suspension::RRTravel;
    }

    bool RL_OCSC()
    {
        return fakes::io::suspension::RLOCSC;
    }

    bool RR_OCSC()
    {
        return fakes::io::suspension::RROCSC;
    }
} // namespace suspension

namespace rPump
{
    std::expected<void, ErrorCode> setPercentage(uint8_t value)
    {
        return {};
    }

    std::expected<void, ErrorCode> readPercentage(uint8_t &dest)
    {
        dest = fakes::io::rPump::percentage;
        return {};
    }
} // namespace rPump

namespace tireTemp
{
    float get()
    {
        return fakes::io::tireTemp::temperature;
    }
} // namespace tireTemp
} // namespace io

static void          overflow_callback() {}
static void          overflow_callback(uint32_t) {}
const io::shdn::node rl_int_3v3_sens(true, app::can_tx::RSM_RearLeftMotorInterlock_set);

io::queue<io::CanMsg, 128> can_tx_queue{ "", overflow_callback, overflow_callback };
io::queue<io::CanMsg, 128> can_rx_queue{ "", overflow_callback, overflow_callback };
