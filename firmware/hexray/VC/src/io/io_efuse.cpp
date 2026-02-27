#include "io_efuse.hpp"

#include "efuse/io_efuse_TI_TPS28.hpp"
#include "hw_adcs.hpp"
#include "hw_gpios.hpp"


io::TI_TPS28_Efuse f_inv(f_inv_en, hw::adcs::adc_f_inv, f_inv_pg, 0);

 // namespace io

