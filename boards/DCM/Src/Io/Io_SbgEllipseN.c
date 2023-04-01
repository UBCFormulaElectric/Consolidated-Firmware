#include "main.h"
#include <assert.h>
#include "sbgECom.h"
#include "interfaces/sbgInterfaceSerial.h"
#include "Io_SbgEllipseN.h"
#include "App_RingQueue.h"
#include "App_SharedMacros.h"

/* ------------------------------------ Defines ------------------------------------- */

#define UART_RX_PACKET_SIZE 128 // Size of each received UART packet, in bytes

/* ------------------------------------ Typedefs ------------------------------------- */

typedef struct
{
    Vector3 acceleration;
    Vector3 angular_acceleration;
} ImuPacketData;

typedef struct
{
    Attitude euler_angles;
} EulerPacketData;

typedef struct
{
    uint32_t timestamp_us;
} UtcTimePacketData;

typedef struct
{
    ImuPacketData     imu_data;
    EulerPacketData   euler_data;
    UtcTimePacketData utc_data;
} GpsData;

/* --------------------------------- Variables ---------------------------------- */

extern UART_HandleTypeDef huart1;

static SbgInterface  sbg_interface;
static SbgEComHandle com_handle;
static uint8_t       uart_rx_buffer[UART_RX_PACKET_SIZE];
static RingQueue     rx_queue;
static GpsData       gps_data;

/* ------------------------- Static Function Prototypes -------------------------- */

static void Io_SbgEllipseN_CreateSerialInterface(SbgInterface *interface);
static SbgErrorCode
                    Io_SbgEllipseN_Read(SbgInterface *interface, void *buffer, size_t *read_bytes, size_t bytes_to_read);
static SbgErrorCode Io_SbgEllipseN_LogReceivedCallback(
    SbgEComHandle *         handle,
    SbgEComClass            msg_class,
    SbgEComMsgId            msg_id,
    const SbgBinaryLogData *log_data,
    void *                  user_arg);
static void Io_SbgEllipseN_ProcessMsg_Imu(SbgEComClass msg_class, SbgEComMsgId msg, const SbgBinaryLogData *log_data);
static void
    Io_SbgEllipseN_ProcessMsg_EulerAngles(SbgEComClass msg_class, SbgEComMsgId msg, const SbgBinaryLogData *log_data);
static void
    Io_SbgEllipseN_ProcessMsg_Status(SbgEComClass msg_class, SbgEComMsgId msg, const SbgBinaryLogData *log_data);
static void
    Io_SbgEllipseN_ProcessMsg_UtcTime(SbgEComClass msg_class, SbgEComMsgId msg, const SbgBinaryLogData *log_data);

/* ------------------------- Static Function Definitions -------------------------- */

/*
 * Callback called when a UART packet is received.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    assert(huart == &huart1);

    // Push newly received data to queue
    for (int i = 0; i < UART_RX_PACKET_SIZE; i++)
    {
        App_RingQueue_Push(&rx_queue, uart_rx_buffer[i]);
    }
}

/*
 * Create a serial interface that will be used by SBG's library. Requires passing pointers to functions for all I/O
 * operations, such as reading, writing, flushing, etc. These I/O operations will use STM32's HAL drivers to communicate
 * to the sensor.
 */
static void Io_SbgEllipseN_CreateSerialInterface(SbgInterface *interface)
{
    sbgInterfaceNameSet(interface, "SBG Ellipse N Sensor");

    interface->type          = SBG_IF_TYPE_UNKNOW;
    interface->pDestroyFunc  = NULL;
    interface->pWriteFunc    = NULL;
    interface->pReadFunc     = Io_SbgEllipseN_Read;
    interface->pFlushFunc    = NULL;
    interface->pSetSpeedFunc = NULL;
    interface->pGetSpeedFunc = NULL;
    interface->pDelayFunc    = NULL;
}

/*
 * Read some data from the GPS sensor via UART.
 */
static SbgErrorCode Io_SbgEllipseN_Read(SbgInterface *interface, void *buffer, size_t *read_bytes, size_t bytes_to_read)
{
    UNUSED(interface);

    vPortEnterCritical(); // Disable interrupts so UART RX callback won't push data while we're popping
    *read_bytes = 0;

    // Read all available data from the RX queue, up to the requested amount
    size_t i = 0;
    while (i < bytes_to_read)
    {
        uint8_t data;
        bool    data_available = App_RingQueue_Pop(&rx_queue, &data);

        if (!data_available)
        {
            break;
        }

        ((uint8_t *)buffer)[i] = data;
        *read_bytes += 1;
        i++;
    }

    vPortExitCritical();
    return SBG_NO_ERROR;
}

/*
 * Callback called when a full log is successfully received and parsed.
 */
SbgErrorCode Io_SbgEllipseN_LogReceivedCallback(
    SbgEComHandle *         handle,
    SbgEComClass            msg_class,
    SbgEComMsgId            msg_id,
    const SbgBinaryLogData *log_data,
    void *                  user_arg)
{
    assert(log_data);

    UNUSED(handle);
    UNUSED(user_arg);

    if (msg_class == SBG_ECOM_CLASS_LOG_ECOM_0)
    {
        switch (msg_id)
        {
            case SBG_ECOM_LOG_IMU_DATA:
            {
                Io_SbgEllipseN_ProcessMsg_Imu(msg_class, msg_id, log_data);
                break;
            }
            case SBG_ECOM_LOG_EKF_EULER:
            {
                Io_SbgEllipseN_ProcessMsg_EulerAngles(msg_class, msg_id, log_data);
                break;
            }
            case SBG_ECOM_LOG_STATUS:
            {
                Io_SbgEllipseN_ProcessMsg_Status(msg_class, msg_id, log_data);
                break;
            }
            case SBG_ECOM_LOG_UTC_TIME:
            {
                Io_SbgEllipseN_ProcessMsg_UtcTime(msg_class, msg_id, log_data);
                break;
            }
            default:
            {
                // Do nothing
                break;
            }
        }
    }

    return SBG_NO_ERROR;
}

/*
 * Process and save a new IMU data msg.
 */
static void Io_SbgEllipseN_ProcessMsg_Imu(SbgEComClass msg_class, SbgEComMsgId msg, const SbgBinaryLogData *log_data)
{
    assert(msg_class == SBG_ECOM_CLASS_LOG_ECOM_0);
    assert(msg == SBG_ECOM_LOG_IMU_DATA);

    // Save acceleration data, in m/s^2
    gps_data.imu_data.acceleration.x = log_data->imuData.accelerometers[0];
    gps_data.imu_data.acceleration.y = log_data->imuData.accelerometers[1];
    gps_data.imu_data.acceleration.z = log_data->imuData.accelerometers[2];

    // Save angular acceleration data, in rad/s^2
    gps_data.imu_data.angular_acceleration.x = log_data->imuData.gyroscopes[0];
    gps_data.imu_data.angular_acceleration.y = log_data->imuData.gyroscopes[1];
    gps_data.imu_data.angular_acceleration.z = log_data->imuData.gyroscopes[2];
}

/*
 * Process and save a new euler angles msg.
 */
static void
    Io_SbgEllipseN_ProcessMsg_EulerAngles(SbgEComClass msg_class, SbgEComMsgId msg, const SbgBinaryLogData *log_data)
{
    assert(msg_class == SBG_ECOM_CLASS_LOG_ECOM_0);
    assert(msg == SBG_ECOM_LOG_EKF_EULER);

    // Save euler angle data, in rad
    gps_data.euler_data.euler_angles.roll  = RAD_TO_DEG(log_data->ekfEulerData.euler[0]);
    gps_data.euler_data.euler_angles.pitch = RAD_TO_DEG(log_data->ekfEulerData.euler[1]);
    gps_data.euler_data.euler_angles.yaw   = RAD_TO_DEG(log_data->ekfEulerData.euler[2]);
}

/*
 * Process and save a new status msg.
 */
static void Io_SbgEllipseN_ProcessMsg_Status(SbgEComClass msg_class, SbgEComMsgId msg, const SbgBinaryLogData *log_data)
{
    assert(msg_class == SBG_ECOM_CLASS_LOG_ECOM_0);
    assert(msg == SBG_ECOM_LOG_STATUS);

    // TODO: Finish this message
}

/*
 * Process and save a UTC time msg.
 */
static void
    Io_SbgEllipseN_ProcessMsg_UtcTime(SbgEComClass msg_class, SbgEComMsgId msg, const SbgBinaryLogData *log_data)
{
    assert(msg_class == SBG_ECOM_CLASS_LOG_ECOM_0);
    assert(msg == SBG_ECOM_LOG_UTC_TIME);

    gps_data.utc_data.timestamp_us = log_data->utcData.timeStamp;
}

/* ------------------------- Public Function Definitions -------------------------- */

bool Io_SbgEllipseN_Init()
{
    memset(&gps_data, 0, sizeof(GpsData));

    // Initialize the SBG serial interface handle
    Io_SbgEllipseN_CreateSerialInterface(&sbg_interface);

    // Init SBG's communication protocol handle
    if (sbgEComInit(&com_handle, &sbg_interface) != SBG_NO_ERROR)
    {
        return false;
    }

    // Set the callback function (callback is called when a new log is successfully received and parsed)
    sbgEComSetReceiveLogCallback(&com_handle, Io_SbgEllipseN_LogReceivedCallback, NULL);

    // Init UART RX queue
    App_RingQueue_Init(&rx_queue, RING_QUEUE_MAX_SIZE);

    // Start waiting for UART packets
    HAL_UART_Receive_DMA(&huart1, uart_rx_buffer, UART_RX_PACKET_SIZE);

    return true;
}

void Io_SbgEllipseN_HandleLogs()
{
    // Handle a single log. Calls the pReadFunc set in sbgInterfaceSerialCreate to get new data and attempts to parse
    // the log. If successful, the the receive log callback function set in init is triggered for each log. If the log
    // is incomplete, the data will be saved to a buffer to be used once more data is received.
    sbgEComHandle(&com_handle);
}

void Io_SbgEllipseN_GetAttitude(Attitude *attitude)
{
    attitude->roll  = gps_data.euler_data.euler_angles.roll;
    attitude->pitch = gps_data.euler_data.euler_angles.pitch;
    attitude->yaw   = gps_data.euler_data.euler_angles.yaw;
}

uint32_t Io_SbgEllipseN_GetTimestamp()
{
    return gps_data.utc_data.timestamp_us;
}
