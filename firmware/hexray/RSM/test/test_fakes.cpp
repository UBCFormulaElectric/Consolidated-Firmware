#include "test_fakes.hpp"
#include "io_brake.hpp"
#include "io_coolant.hpp"
#include "io_suspension.hpp"
#include "io_tireTemp.hpp"
#include "io_imus.hpp"

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
    ExitCode setPercentage(float value)
    {
        return ExitCode::EXIT_CODE_OK;
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
