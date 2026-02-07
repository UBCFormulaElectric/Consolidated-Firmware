#include "chimera_v2.hpp"
#include "tasks.h"
#include "hw_gpios.hpp"
#include "hw_adcs.hpp"
#include "hw_i2cs.hpp"
#include "hw_spis.hpp"
#include "hw_usb.hpp"
#include "hw_rtosTaskHandler.hpp"
#include <rsm.pb.h>
#include <cassert>

#include <optional>
#include <functional>

class RSMChimeraConfig : public chimera_v2::config
{
  public:
    ~RSMChimeraConfig() override = default;
    std::optional<std::reference_wrapper<const hw::Gpio>> id_to_gpio(const _GpioNetName *gnn) const override
    {
        if (gnn->which_name != gpio_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected GPIO net name with tag %d, got %d", gpio_net_name_tag, gnn->which_name);
            return std::nullopt;
        }
        switch (gnn->name.rsm_net_name)
        {
            case rsm_GpioNetName_GPIO_LED:
                return std::cref(hw::gpio::led);
            case rsm_GpioNetName_GPIO_BRAKE_LIGHT_EN:
                return std::cref(hw::gpio::brake_light_en);
            case rsm_GpioNetName_GPIO_RL_INT:
                return std::cref(hw::gpio::rl_int);
            case rsm_GpioNetName_GPIO_IMU_CS:
                return std::cref(hw::gpio::imu_cs);
            case rsm_GpioNetName_GPIO_IMU_INT:
                return std::cref(hw::gpio::imu_int);
            case rsm_GpioNetName_GPIO_IMU_FSYNC:
                return std::cref(hw::gpio::imu_fsync);
            case rsm_GpioNetName_GPIO_SUSP_TRAVEL_RR_OCSC:
                return std::cref(hw::gpio::susp_travel_rr_ocsc);
            case rsm_GpioNetName_GPIO_SUSP_TRAVEL_RL_OCSC:
                return std::cref(hw::gpio::susp_travel_rl_ocsc);
            case rsm_GpioNetName_GPIO_BRAKE_OCSC_OK:
                return std::cref(hw::gpio::brake_ocsc_ok);
            case rsm_GpioNetName_GPIO_D_P_PULLUP:
                return std::cref(hw::gpio::d_p_pullup);
            default:
            case rsm_GpioNetName_GPIO_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unspecified GPIO net name");
                return std::nullopt;
        }
    }
    std::optional<std::reference_wrapper<const hw::Adc>> id_to_adc(const _AdcNetName *ann) const override
    {
        if (ann->which_name != adc_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected ADC netname with tag %d, got %d", adc_net_name_tag, ann->which_name);
            return std::nullopt;
        }
        switch (ann->name.rsm_net_name)
        {
            case rsm_AdcNetName_ADC_LC3_OUT:
                return std::cref(hw::adcs::lc3_out);
            case rsm_AdcNetName_ADC_SUSP_TRAVEL_RL_3V3:
                return std::cref(hw::adcs::susp_travel_rl_3v3);
            case rsm_AdcNetName_ADC_SUSP_TRAVEL_RR_3V3:
                return std::cref(hw::adcs::susp_travel_rr_3v3);
            case rsm_AdcNetName_ADC_BPS_3V3:
                return std::cref(hw::adcs::bps_3v3);
            case rsm_AdcNetName_ADC_nBSPD_BRAKE_PRESSED:
                return std::cref(hw::adcs::nBSPD_brake_pressed);
            default:
            case rsm_AdcNetName_ADC_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unspecified ADC net name");
                return std::nullopt;
        }
    }
    std::optional<std::reference_wrapper<const hw::i2c::I2CDevice>> id_to_i2c(const _I2cNetName *inn) const override
    {
        if (inn->which_name != i2c_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected I2C netname with tag %d, got %d", i2c_net_name_tag, inn->which_name);
            return std::nullopt;
        }
        switch (inn->name.rsm_net_name)
        {
            case rsm_I2cNetName_I2C_R_PUMP:
                return std::cref(hw::i2c::r_pump);
            default:
            case rsm_I2cNetName_I2C_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unspecified I2C net name");
                return std::nullopt;
        }
    }
    std::optional<std::reference_wrapper<const hw::spi::SpiDevice>> id_to_spi(const _SpiNetName *snn) const override
    {
        if (snn->which_name != spi_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected SPI netname with tag %d, got %d", spi_net_name_tag, snn->which_name);
            return std::nullopt;
        }
        switch (snn->name.rsm_net_name)
        {
            case rsm_SpiNetName_SPI_IMU:
                return std::cref(hw::spi::imu);
            default:
            case rsm_SpiNetName_SPI_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unspecified SPI net name");
                return std::nullopt;
        }
    }
    RSMChimeraConfig()
    {
        gpio_net_name_tag = GpioNetName_rsm_net_name_tag;
        adc_net_name_tag  = AdcNetName_rsm_net_name_tag;
        i2c_net_name_tag  = I2cNetName_rsm_net_name_tag;
        spi_net_name_tag  = SpiNetName_rsm_net_name_tag;
    }
} rsm_config;

static hw::rtos::StaticTask<8096>
    TaskChimera(osPriorityRealtime, "TaskChimera", [](void *) { chimera_v2::task(rsm_config); });

void tasks_preInit() {}
char USBD_PRODUCT_STRING_FS[] = "rsm";

[[noreturn]] void tasks_init()
{
    assert(hw::usb::init());
    osKernelInitialize();
    TaskChimera.start();
    osKernelStart();
    forever {}
}

// what is a protobuf generated tags
// how does this system actually work with python