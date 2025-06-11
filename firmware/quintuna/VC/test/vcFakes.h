#pragma once

namespace fake
{
namespace io_vcShdn
{
    void setTsmsFault(bool fault);
    void setInertiaSwitch(bool switchState);
    void setRearRightMotorInterlock(bool interlock);
    void setSplitterBoxInterlockFault(bool fault);
} // namespace io_vcShdn
namespace io_vc
{
}
} // namespace fake
