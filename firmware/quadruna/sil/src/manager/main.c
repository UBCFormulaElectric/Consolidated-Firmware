#include <stdio.h>
#include <stdlib.h>
#include <czmq.h>

#include "sil_board.h"
#include "sil_manager.h"

int main()
{
    // Setup boards.
    sil_Board  fsm      = sil_board_new("FSM", "./build_fw_sil/firmware/quadruna/FSM/quadruna_FSM_sil");
    sil_Board  rsm      = sil_board_new("RSM", "./build_fw_sil/firmware/quadruna/RSM/quadruna_RSM_sil");
    sil_Board  bms      = sil_board_new("DIM", "./build_fw_sil/firmware/quadruna/BMS/quadruna_BMS_sil");
    sil_Board  crit     = sil_board_new("CRIT", "./build_fw_sil/firmware/quadruna/CRIT/quadruna_CRIT_sil");
    sil_Board  vc       = sil_board_new("VC", "./build_fw_sil/firmware/quadruna/VC/quadruna_VC_sil");
    sil_Board *boards[] = { &fsm, &rsm, &bms, &crit, &vc, NULL };

    // Start up manager.
    sil_manager_start(NULL, boards);

    // Quick demo.

    int64_t startMs = zclock_mono();
    sil_manager_setTime(1000, boards);
    printf("Real Time: %lld\n", zclock_mono() - startMs);

    printf("--- Sleeping for 1 s ---\n");
    zclock_sleep(1000);

    startMs = zclock_mono();
    sil_manager_setTime(2000, boards);
    printf("Real Time: %lld\n", zclock_mono() - startMs);

    printf("--- Resetting in 1 s ---\n");
    zclock_sleep(1000);

    sil_manager_start(boards, boards);

    printf("--- Starting a 1 s after a 1 s pause ---\n");
    zclock_sleep(1000);

    startMs = zclock_mono();
    sil_manager_setTime(1000, boards);
    printf("Real Time: %lld\n", zclock_mono() - startMs);
}