#pragma once

namespace vc::app::loadswitches{

extern enum class EfuseChannel : uint8_t {
  F_INV = 0x0U,
  R_INV,
  RSM,
  BMS,
  DAM,
  FRONT, //FSM and CRIT
  MISC,
  //these are not on the loadswitch board tho
  RR_PUMP,// Are these two considered MISC?? 
  RL_PUMP,
  R_RAD,
  L_RAD,
  Count
};

static const void efuse_channel_setter (EfuseChannel *channel, bool status);
static const void efuse_current_setter (EfuseChannel &channel, float current);

} // namespace vc::app::loadswitches