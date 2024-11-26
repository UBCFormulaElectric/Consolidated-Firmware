#include "tasks.h"

#include <cstdint>
#include "cmsis_os.h"

extern "C"
{
#include "io_can.h"
#include "io_canLogging.h"
#include "io_log.h"
#include "io_fileSystem.h"
#include "hw_can.h"
}

static int write_num = 0;
static int read_num  = 0;

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("h7dev reset!");
}

void tasks_default()
{
    /* Infinite loop */
    for (;;)
    {
        // UART modem_uart = { .handle = &huart9 };
        // /* Infinite loop */
        // // uint8_t message[7] = { 66, 79, 79, 66, 83, 13, 10 };
        // // uint8_t num; // use this if just want numbers
        // // uint8_t predicData[3];
        // // predicData[1] = 13;
        // // predicData[2] = 10;
        // uint8_t buffer[128];
        // uint8_t message_length;
        // bool    status;

        // // SimpleMessage message = SimpleMessage_init_zero;

        // for (;;)
        // {
        //     /* Create a stream that will write to our buffer. */
        //     TelemMessage message = TelemMessage_init_zero;
        //     pb_ostream_t stream  = pb_ostream_from_buffer(buffer, sizeof(buffer));

        //     /* Fill in the lucky number */
        //     message.can_id     = 53;
        //     message.data       = 23;
        //     message.time_stamp = 9;

        //     /* Now we are ready to encode the message! */
        //     status         = pb_encode(&stream, TelemMessage_fields, &message);
        //     message_length = (uint8_t)stream.bytes_written;
        //     // message_length = stream.bytes_written;
        //     if (status)
        //     {
        //         hw_uart_transmitPoll(&modem_uart, &message_length, 1, 100);
        //         hw_uart_transmitPoll(&modem_uart, buffer, sizeof(buffer), 100); // fun string
        //     }
        //     osDelay(10);
        // }
        osDelay(osWaitForever);
    }
}

void tasks_canRx()
{
    // static uint32_t count = 0;
    for (;;)
    {
        CanMsg msg;
        io_can_popRxMsgFromQueue(&msg);
        // io_canLogging_recordMsgFromQueue();
        // write_num++;
        // count++;
        //
        // if (count > 256)
        // {
        //     io_canLogging_sync();
        //     count = 0;
        // }
    }
}

void tasks_canTx()
{
    for (uint32_t i = 0;; i++)
    {
        uint8_t i8[4];
        *reinterpret_cast<uint32_t *>(&i8) = i;
        CanMsg msg = { .std_id = 0, .dlc = 8, .data = { i8[3], i8[2], i8[1], i8[0], 0, 0, 0, 0 } };
        // for (int j = 0; j < 6; j++) {
        // read_num++;
        // io_canLogging_pushTxMsgToQueue(&msg);
        // }
        // io_can_pushTxMsgToQueue(&msg);

        hw_can_transmit(&msg);
        osDelay(1);
    }
}
