#ifndef BOARD_PROCEDURES_H
#define BOARD_PROCEDURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void set_pedal_percentage_50();
void turn_BMS_light_on();

// array for function mapping
extern sil_api_Procedure          VC_functions[];
extern sil_api_Procedure          BMS_functions[];
extern const BoardFunctionMapping board_tables[];

#endif