#include "test_fakes.hpp"


#include "app_tractiveSystem.hpp"
#include "app_segments.hpp"
#include "io_irs.hpp"

namespace app
{

namespace ts
{
    static float voltage = 0.0f;

    float getVoltage()
    {
        return voltage;
    }
}

namespace segments
{
namespace shared
{
    static result<float> packVoltage = 600.0f;

    result<float> getPackVoltage(){
        return packVoltage;
    }
}
}

}

namespace io
{

namespace irs
{
    static auto negative_state  = app::can_utils::ContactorState::CONTACTOR_STATE_OPEN;

    app::can_utils::ContactorState negativeState()
    {
        return negative_state;
    }
}
}


namespace fakes
{
namespace irs
{
    void setNegativeState(const app::can_utils::ContactorState state)
    {
        io::irs::negative_state = state;
    }
} // namespace irs

namespace ts
{
    void setTsVoltage(const float tsVoltage)
    {
        app::ts::voltage = tsVoltage;
    }
} // namespace ts

namespace segments
{
    void setPackVoltage(const float packVoltage)
    {
        app::segments::shared::packVoltage = result<packVoltage>;
    }
} // namespace segments

} // namespace fakes
