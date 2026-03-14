#ifdef TARGET_EMBEDDED
#include "hw_adcs.hpp"
#include "hw_gpios.hpp"
#endif
#include "io_efuse_TI_TPS25.hpp"
#include "io_efuse_TI_TPS28.hpp"
#include "io_efuse.hpp"

namespace io::loadswitches
{
bool isChannelEnabled(io::TPS25_EfuseChannel channel);
bool TILoadswitch_pgood(const *temp);

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

} // namespace io::loadswitches