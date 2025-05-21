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
} // namespace fake

extern "C"
{
#include "io_loadSwitches.h"
    static const Efuse f_inv_efuse{};
    static const Efuse rsm_efuse{};
    static const Efuse bms_efuse{};
    static const Efuse r_inv_efuse{};
    static const Efuse dam_efuse{};
    static const Efuse front_efuse{};
    static const Efuse rl_pump_efuse{};
    static const Efuse rr_pump_efuse{};
    static const Efuse f_pump_efuse{};
    static const Efuse l_rad_fan_efuse{};
    static const Efuse r_rad_fan_efuse{};

    const ST_LoadSwitch inv_rsm_loadswitch                 = { .efuse1 = &f_inv_efuse, .efuse2 = &rsm_efuse };
    const ST_LoadSwitch inv_bms_loadswitch                 = { .efuse1 = &bms_efuse, .efuse2 = &r_inv_efuse };
    const ST_LoadSwitch front_loadswitch                   = { .efuse1 = &dam_efuse, .efuse2 = &front_efuse };
    const TI_LoadSwitch rl_pump_loadswitch                 = { .efuse = &rl_pump_efuse };
    const TI_LoadSwitch rr_pump_loadswitch                 = { .efuse = &rr_pump_efuse };
    const TI_LoadSwitch f_pump_loadswitch                  = { .efuse = &f_pump_efuse };
    const ST_LoadSwitch rad_fan_loadswitch                 = { .efuse1 = &l_rad_fan_efuse, .efuse2 = &r_rad_fan_efuse };
    const Efuse *const  efuse_channels[NUM_EFUSE_CHANNELS] = {
        [EFUSE_CHANNEL_F_INV] = &f_inv_efuse,     [EFUSE_CHANNEL_RSM] = &rsm_efuse,
        [EFUSE_CHANNEL_BMS] = &bms_efuse,         [EFUSE_CHANNEL_R_INV] = &r_inv_efuse,
        [EFUSE_CHANNEL_DAM] = &dam_efuse,         [EFUSE_CHANNEL_FRONT] = &front_efuse,
        [EFUSE_CHANNEL_RL_PUMP] = &rl_pump_efuse, [EFUSE_CHANNEL_RR_PUMP] = &rr_pump_efuse,
        [EFUSE_CHANNEL_F_PUMP] = &f_pump_efuse,   [EFUSE_CHANNEL_L_RAD] = &l_rad_fan_efuse,
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
}
