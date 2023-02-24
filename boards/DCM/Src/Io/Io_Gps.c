#include "main.h"
#include <assert.h>
#include "Io_Gps.h"
#include "sbgECom.h"
#include "interfaces/sbgInterfaceSerial.h"

extern UART_HandleTypeDef huart1;

// Probably best to do this via an interrupt or DMA so we don't have to block to read from the sensor

SbgErrorCode Io_Gps_SbgDestroy(SbgInterface *interface)
{
    // TODO: Implement
    return SBG_NO_ERROR;
}

SbgErrorCode Io_Gps_SbgRead(SbgInterface *interface, void *buffer, size_t *read_bytes, size_t bytes_to_read)
{
    uint8_t rx_data[bytes_to_read];
    HAL_UART_Receive_IT(&huart1, rx_data, (uint16_t)bytes_to_read);

    return SBG_NO_ERROR;
}

SbgErrorCode Io_Gps_SbgWrite(SbgInterface *interface, const void *buffer, size_t bytes_to_write)
{
    // TODO: Implement
    return SBG_NO_ERROR;
}

SbgErrorCode Io_Gps_SbgFlush(SbgInterface *interface, uint32_t flags)
{
    // TODO: Implement
    return SBG_NO_ERROR;
}

SbgErrorCode Io_Gps_SbgOnLogReceived(
    SbgEComHandle *         handle,
    SbgEComClass            msg_class,
    SbgEComMsgId            msg,
    const SbgBinaryLogData *log_data,
    void *                  user_arg)
{
    // TODO: Implement
    return SBG_NO_ERROR;
}

SbgErrorCode sbgInterfaceSerialCreate(SbgInterface *interface, const char *deviceName, uint32_t baud_rate)
{
    assert(baud_rate == SBG_BAUD_RATE);
    sbgInterfaceNameSet(interface, deviceName);

    interface->handle       = NULL;
    interface->type         = SBG_IF_TYPE_UNKNOW;
    interface->pDestroyFunc = Io_Gps_SbgDestroy;
    interface->pReadFunc    = Io_Gps_SbgRead;
    interface->pWriteFunc   = Io_Gps_SbgWrite;
    interface->pFlushFunc   = Io_Gps_SbgFlush;

    return Io_Gps_SbgFlush(interface, SBG_IF_FLUSH_ALL);
}

void Io_Gps_Init()
{
    SbgErrorCode  err_code = SBG_NO_ERROR;
    SbgInterface  sbg_interface;
    SbgEComHandle com_handle;

    // Init SBG interface handle
    err_code = sbgInterfaceSerialCreate(&sbg_interface, "SBG Ellipse N", SBG_BAUD_RATE);

    // Init SBG communication handle
    err_code = sbgEComInit(&com_handle, &sbg_interface);

    // Set the callback function (called when log received and parsed)
    sbgEComSetReceiveLogCallback(&com_handle, Io_Gps_SbgOnLogReceived, NULL);

    // Handle a single log. Calls the pReadFunc set in sbgInterfaceSerialCreate to get the log to parse, then
    // parses the log into an SbgBinaryLogData object, which is passed to the callback function.
    err_code = sbgEComHandleOneLog(&com_handle);

    (void)err_code;
}
