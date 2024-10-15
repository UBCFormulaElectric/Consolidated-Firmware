#pragma once

#include <stdbool.h>

extern "C"
{
#include "app_utils.h"
}

#include "hw_gpio.h"
#include "hw_adc.h"

namespace io::efuse
{
class Efuse
{
#ifdef TARGET_EMBEDDED
  private:
    const Gpio *enable_gpio;
    const Gpio *stby_reset;
    const Adc  *cur_sns_channel;

  public:
    explicit Efuse(const Gpio *enable_in, const Gpio *stby_reset_in, const Adc *cur_sns_channel_in)
      : enable_gpio(enable_in), stby_reset(stby_reset_in), cur_sns_channel(cur_sns_channel_in)
    {
    }
#endif

    enum class EfuseChannel
    {
        // FR_STBY1
        EFUSE_CHANNEL_SHDN,
        EFUSE_CHANNEL_LV,

        // FR_STBY2
        EFUSE_CHANNEL_PUMP,
        EFUSE_CHANNEL_AUX,

        // FR_STBY3
        EFUSE_CHANNEL_INV_R,
        EFUSE_CHANNEL_INV_L,

        // No STBY, no SNS
        EFUSE_CHANNEL_TELEM,
        EFUSE_CHANNEL_BUZZER,

        NUM_EFUSE_CHANNELS
    };

    void  setChannel(EfuseChannel channel, bool enabled);
    bool  isChannelEnabled(EfuseChannel channel);
    float getChannelCurrent(EfuseChannel channel);
    void  standbyReset(EfuseChannel channel);
};
} // namespace io::efuse
