#pragma once

#include <stdbool.h>
#include "App_InvWorld.h"

bool App_Faults_FaultedNonCritical(
    const struct InvCanTxInterface *can_tx_interface);
bool App_Faults_FaultedMotorShutdown(
    const struct InvCanTxInterface *can_tx_interface,
    const struct InvCanRxInterface *can_rx_interface);