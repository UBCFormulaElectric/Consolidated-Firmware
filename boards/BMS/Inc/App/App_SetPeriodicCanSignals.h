#pragma once

struct BmsCanTxInterface;
struct Imd;

void App_SetPeriodicCanSignals_Imd(
    struct BmsCanTxInterface *can_tx,
    struct Imd *              imd);
