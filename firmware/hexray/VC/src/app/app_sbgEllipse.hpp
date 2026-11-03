#pragma once

#include <cstdint>
#include "io_sbgEllipse.hpp"
#include "app_canUtils.hpp"

namespace app::sbgEllipse
{
void init();

/*
 * Broadcast sensor outputs over CAN.
 */
void broadcast(void);
/*
 * Check if the Sbg Ellipsed Initialized properly
 */
bool sbgInitOk();

/**
 * Body Velocity from SBG ellipse
 */
float bodyVelX();

float bodyVelY();

float bodyVelZ();

/**
 * Global Velocity from SBG ellipse
 */
float globalVelN();

float globalVelE();

float globalVelD();

/*
 * Get Ekf Solution mode
 *
 * POSITION represents the highest accuracy mode
 */
can_utils::VcEkfStatus getEkfSolutionMode(void);
} // namespace app::sbgEllipse
