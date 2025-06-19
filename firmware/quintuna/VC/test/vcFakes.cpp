#include <stdint.h>

#include "vcFakes.h"

namespace fake
{
namespace io_vcShdn
{
    static bool tsmsFault                 = false;
    static bool inertiaSwitch             = false;
    static bool rearRightMotorInterlock   = false;
    static bool splitterBoxInterlockFault = false;

    void setTsmsFault(const bool fault)
    {
        tsmsFault = fault;
    }
    void setInertiaSwitch(const bool switchState)
    {
        inertiaSwitch = switchState;
    }
    void setRearRightMotorInterlock(const bool interlock)
    {
        rearRightMotorInterlock = interlock;
    }
    void setSplitterBoxInterlockFault(const bool fault)
    {
        splitterBoxInterlockFault = fault;
    }
} // namespace io_vcShdn
namespace io_vc
{
}

namespace io_sbgEllipse
{
    static uint32_t ekf_solution_mode;
    void setEkfSolutionMode(const uint32_t ekf_solution_mode)
    {
        io_sbgEllipse::ekf_solution_mode = ekf_solution_mode;
    }
}
} // namespace fake

extern "C"
{
#include "io_loadswitches.h"
    static Efuse f_inv_efuse{};
    static Efuse rsm_efuse{};
    static Efuse bms_efuse{};
    static Efuse r_inv_efuse{};
    static Efuse dam_efuse{};
    static Efuse front_efuse{};
    static Efuse rl_pump_efuse{};
    static Efuse rr_pump_efuse{};
    static Efuse f_pump_efuse{};
    static Efuse l_rad_fan_efuse{};
    static Efuse r_rad_fan_efuse{};

    ST_LoadSwitch      inv_rsm_loadswitch                 = { .efuse1 = &f_inv_efuse, .efuse2 = &rsm_efuse };
    ST_LoadSwitch      inv_bms_loadswitch                 = { .efuse1 = &bms_efuse, .efuse2 = &r_inv_efuse };
    ST_LoadSwitch      front_loadswitch                   = { .efuse1 = &dam_efuse, .efuse2 = &front_efuse };
    TI_LoadSwitch      rl_pump_loadswitch                 = { .efuse = &rl_pump_efuse };
    TI_LoadSwitch      rr_pump_loadswitch                 = { .efuse = &rr_pump_efuse };
    TI_LoadSwitch      f_pump_loadswitch                  = { .efuse = &f_pump_efuse };
    ST_LoadSwitch      rad_fan_loadswitch                 = { .efuse1 = &l_rad_fan_efuse, .efuse2 = &r_rad_fan_efuse };
    const Efuse *const efuse_channels[NUM_EFUSE_CHANNELS] = {
        [EFUSE_CHANNEL_F_INV] = &f_inv_efuse,     [EFUSE_CHANNEL_RSM] = &rsm_efuse,
        [EFUSE_CHANNEL_BMS] = &bms_efuse,         [EFUSE_CHANNEL_R_INV] = &r_inv_efuse,
        [EFUSE_CHANNEL_DAM] = &dam_efuse,         [EFUSE_CHANNEL_FRONT] = &front_efuse,
        [EFUSE_CHANNEL_RL_PUMP] = &rl_pump_efuse, [EFUSE_CHANNEL_L_RAD] = &l_rad_fan_efuse,
        [EFUSE_CHANNEL_R_RAD] = &r_rad_fan_efuse
    };

#include "io_vcShdn.h"
    bool io_vcShdn_TsmsFault_get(void)
    {
        return fake::io_vcShdn::tsmsFault;
    }
    bool io_vcShdn_InertiaSwitch_get(void)
    {
        return fake::io_vcShdn::inertiaSwitch;
    }
    bool io_vcShdn_RearRightMotorInterlock_get(void)
    {
        return fake::io_vcShdn::rearRightMotorInterlock;
    }
    bool io_vcShdn_SplitterBoxInterlockFault_get(void)
    {
        return fake::io_vcShdn::splitterBoxInterlockFault;
    }

#include "io_canQueues.h"
    CanTxQueue can1_tx_queue;
    CanTxQueue can2_tx_queue;
    CanTxQueue can3_tx_queue;

#include "io_pcm.h"
    static bool pcm_enabled = false;

    void io_pcm_set(const bool enable)
    {
        pcm_enabled = enable;
    }
    bool io_pcm_enabled(void)
    {
        return pcm_enabled;
    }

#include "io_powerMonitoring.h"
    bool io_powerMonitoring_init(void)
    {
        return true;
    }
    void io_powerMonitoring_refresh(void) {}

    void io_powerMonitoring_read_voltage(const uint8_t ch, float *voltage)
    {
        UNUSED(ch);
        *voltage = 0.0f;
    }
    void io_powerMonitoring_read_current(const uint8_t ch, float *current)
    {
        UNUSED(ch);
        *current = 0.0f;
    }
    void io_powerMonitoring_read_power(const uint8_t ch, float *power)
    {
        UNUSED(ch);
        *power = 0.0f;
    }

#include "io_sbgEllipse.h"
    ExitCode io_sbgEllipse_init()
    {
        return EXIT_CODE_OK;
    }
    void     io_sbgEllipse_handleLogs(void) {}
    uint32_t io_sbgEllipse_getTimestampUs(void)
    {
        return 0;
    }
    uint16_t io_sbgEllipse_getGeneralStatus(void)
    {
        return 0;
    }
    uint32_t io_sbgEllipse_getComStatus(void)
    {
        return 0;
    }
    uint32_t io_sbgEllipse_getOverflowCount(void)
    {
        return 0;
    }
    uint32_t io_sbgEllipse_getEkfSolutionMode(void)
    {
        return 4;
    }

    static Vector3 imu_accelerations;
    Vector3       *io_sbgEllipse_getImuAccelerations(void)
    {
        return &imu_accelerations;
    }

    static Attitude imu_attitude;
    Attitude       *io_sbgEllipse_getImuAngularVelocities(void)
    {
        return &imu_attitude;
    }

    static Attitude ekf_attitude;
    Attitude       *io_sbgEllipse_getEkfEulerAngles(void)
    {
        return &ekf_attitude;
    }

    static VelocityData ekf_velocity_data;
    VelocityData       *io_sbgEllipse_getEkfNavVelocityData(void)
    {
        return &ekf_velocity_data;
    }

    static PositionData ekf_position_data;
    PositionData       *io_sbgEllipse_getEkfNavPositionData(void)
    {
        return &ekf_position_data;
    }
}
