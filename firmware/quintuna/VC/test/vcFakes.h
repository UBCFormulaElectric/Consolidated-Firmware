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

namespace io_sbgEllipse
{
    void setEkfSolutionMode(uint32_t ekf_solution_mode);
} // namespace io_sbgEllipse

} // namespace fake
