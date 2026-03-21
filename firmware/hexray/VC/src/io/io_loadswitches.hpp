#include "io_efuse_TI_TPS25.hpp"
#include "io_efuse_TI_TPS28.hpp"
#include "io_efuse.hpp"

// Used to indicate the indeces of the array for better readability
// when setting CAN signals for each efuse channel
enum EfuseChannel
{
    F_INV = 0,
    RSM,
    BMS,
    R_INV,
    DAM,
    FRONT,
    RL_PUMP,
    R_RAD,
    RR_PUMP,
    L_RAD
};

// Actual efuse handlers
extern io::TI_TPS25_Efuse RR_PUMP_Efuse;
extern io::TI_TPS25_Efuse RL_PUMP_Efuse;
extern io::TI_TPS25_Efuse R_RAD_Efuse;
extern io::TI_TPS25_Efuse L_RAD_Efuse;
extern io::TI_TPS28_Efuse F_INV_Efuse;
extern io::TI_TPS28_Efuse R_INV_Efuse;
extern io::TI_TPS28_Efuse RSM_Efuse;
extern io::TI_TPS28_Efuse BMS_Efuse;
extern io::TI_TPS28_Efuse DAM_Efuse;
extern io::TI_TPS28_Efuse FRONT_Efuse;