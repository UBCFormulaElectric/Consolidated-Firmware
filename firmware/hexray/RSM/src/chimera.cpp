#include "chimera_v2.hpp"
#include "tasks.h"

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
          return std::cref(led);
        case rsm_GpioNetName_GPIO_BRAKE_LIGHT_EN:
          return std::cref(brake_light_en);
        case rsm_GpioNetName_GPIO_RL_INT:
          return std::cref(rl_int);
        case rsm_GpioNetName_GPIO_IMU_CS:
          return std::cref(imu_cs);
        case rsm_GpioNetName_GPIO_IMU_INT:
          return std::cref(imu_int);
        case rsm_GpioNetName_GPIO_IMU_FSYNC:
          return std::cref(imu_fsync);
        case rsm_GpioNetName_GPIO_SUSP_TRAVEL_RR_OCSC:
          return std::cref(susp_travel_rr_ocsc);
        case rsm_GpioNetName_GPIO_SUSP_TRAVEL_RL_OCSC:
          return std::cref(susp_travel_rl_ocsc);
        case rsm_GpioNetName_BRAKE_OCSC_OK:
          return std::cref(brake_ocsc_ok);
        case rsm_GpioNetName_GPIO_D_P_PULLUP:
          return std::cref(d_p_pullup);
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
        LOG_ERROR("Chimera: Expected ADC netname with tag %d, got %d", ann->which_name, adc_net_name_tag)
        return std::nullopt;
      }
      switch (ann->name.rsm_net_name)
      {
        case rsm_AdcNetName_ADC_LC3_OUT:
          return std::cref(lc3_out);
        case rsm_AdcNetName_ADC_SUSP_TRAVEL_RL_3V3:
          return std::cref(susp_travel_rl_3v3);
        case rsm_AdcNetName_ADC_SUSP_TRAVEL_RR_3V3:
          return std::cref(susp_travel_rr_3v3);
        case rsm_AdcNetName_ADC_BPS_3V3:
          return std::cref(bps_3v3);
        case rsm_AdcNetName_ADC_nBSPD_BRAKE_PRESSED:
          return std::cref(nBSPD_brake_pressed);
        default:
        case rsm_AdcNetName_ADC_NET_NAME_UNSPECIFIED:
          return LOG_INFO("Chimera: Unspecified ADC net name");
          return std::nullopt;
      }
    }

    public:
      RSMChimeraConfig()
      {
        gpio_net_name_tag = GpioNetName_rsm_net_name_tag;
        adc_net_name_tag = AdcNetName_rsm_net_name_tag;
      }
} rsm_config;

void tasks_preInit() {}
void tasks_init()
{
    chimera_v2::task(rsm_config);
}

//what is a protobuf generated tags
//how does this system actually work with python 