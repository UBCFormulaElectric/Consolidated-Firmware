#pragma once
#include "hw_uart.h"
#include "../../../build_fw_deploy/firmware/quadruna/VC/telem.pb.h"
#include "../../nanopb/pb_encode.h"
#include "../../nanopb/pb_decode.h"
#include "cmsis_os.h"
#include "queue.h"

/**
 * Creates a new message queue for storing telem messages
 *
 */
void io_telemMessage_init();

/**
 * Serializes the can msg and sends it over UART to the appropriate Modem
 */

void io_telemMessage_broadcast(UART *modem);