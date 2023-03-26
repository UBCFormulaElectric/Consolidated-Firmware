#include "main.h"
#include <assert.h>
#include "sbgECom.h"
#include "interfaces/sbgInterfaceSerial.h"
#include "Io_SbgEllipseGps.h"

/* ------------------------------------ Defines ------------------------------------- */

#define UART_RX_PACKET_SIZE 512 // Size of each received UART packet, in bytes

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
    ImuPacketData   imu_data;
    EulerPacketData euler_data;
} GpsData;

/* --------------------------------- Variables ---------------------------------- */

extern UART_HandleTypeDef huart1;

static SbgInterface  sbg_interface;
static SbgEComHandle com_handle;
static uint8_t       uart_rx_buffer[UART_RX_PACKET_SIZE];
static GpsData       gps_data;
static bool data_available;

/* ------------------------- Static Function Prototypes -------------------------- */

static void Io_SbgEllipseGps_UartRxCallback(void);
static void Io_SbgEllipseGps_ReceiveUartPacket(void);
static void Io_SbgEllipseGps_CreateSerialInterface(SbgInterface *interface);
static SbgErrorCode
                    Io_SbgEllipseGps_Read(SbgInterface *interface, void *buffer, size_t *read_bytes, size_t bytes_to_read);
static SbgErrorCode Io_SbgEllipseGps_Write(SbgInterface *interface, const void *buffer, size_t bytes_to_write);
static SbgErrorCode Io_SbgEllipseGps_LogReceivedCallback(
    SbgEComHandle *         handle,
    SbgEComClass            msg_class,
    SbgEComMsgId            msg_id,
    const SbgBinaryLogData *log_data,
    void *                  user_arg);
static void Io_SbgEllipseGps_ProcessImuMsg(SbgEComClass msg_class, SbgEComMsgId msg, const SbgBinaryLogData *log_data);
static void
    Io_SbgEllipseGps_ProcessEulerAnglesMsg(SbgEComClass msg_class, SbgEComMsgId msg, const SbgBinaryLogData *log_data);
static void
    Io_SbgEllipseGps_ProcessStatusMsg(SbgEComClass msg_class, SbgEComMsgId msg, const SbgBinaryLogData *log_data);

/* ------------------------- Static Function Definitions -------------------------- */

/*
 * Callback called when a UART packet is received.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    assert(huart == &huart1);

    Io_SbgEllipseGps_UartRxCallback();
}

static void Io_SbgEllipseGps_UartRxCallback()
{
    data_available = true;

    // Start waiting for UART packets again
    Io_SbgEllipseGps_ReceiveUartPacket();
}

/*
 * Start receiving the next UART packet. HAL_UART_RxCpltCallback will be triggered when a full packet is received.
 */
static void Io_SbgEllipseGps_ReceiveUartPacket(void)
{
    HAL_UART_Receive_DMA(&huart1, uart_rx_buffer, UART_RX_PACKET_SIZE);
}

/*
 * Create a serial interface that will be used by SBG's library. Requires passing pointers to functions for all I/O
 * operations, such as reading, writing, flushing, etc. These I/O operations will use STM32's HAL drivers to communicate
 * to the sensor.
 */
static void Io_SbgEllipseGps_CreateSerialInterface(SbgInterface *interface)
{
    sbgInterfaceNameSet(interface, "SBG Ellipse N Sensor");

    interface->handle        = NULL;
    interface->handle        = NULL;
    interface->type          = SBG_IF_TYPE_UNKNOW;
    interface->pDestroyFunc  = NULL;
    interface->pWriteFunc    = Io_SbgEllipseGps_Write;
    interface->pReadFunc     = Io_SbgEllipseGps_Read;
    interface->pFlushFunc    = NULL; // TODO: Do we need this?
    interface->pSetSpeedFunc = NULL;
    interface->pGetSpeedFunc = NULL;
    interface->pDelayFunc    = NULL;
}

/*
 * Read some data from the GPS sensor via UART.
 */
static SbgErrorCode
    Io_SbgEllipseGps_Read(SbgInterface *interface, void *buffer, size_t *read_bytes, size_t bytes_to_read)
{
    UNUSED(interface);

    if(!data_available)
    {
        *read_bytes = 0;
        return SBG_ERROR;
    }

    if (bytes_to_read < UART_RX_PACKET_SIZE)
    {
        return SBG_ERROR;
    }

    memcpy(buffer, uart_rx_buffer, sizeof(uint8_t) * UART_RX_PACKET_SIZE);
    *read_bytes = UART_RX_PACKET_SIZE;
    data_available = false;
    return SBG_NO_ERROR;
}

/*
 * Write some data to the GPS sensor via UART.
 */
static SbgErrorCode Io_SbgEllipseGps_Write(SbgInterface *interface, const void *buffer, size_t bytes_to_write)
{
    // TODO: Is this necessary?
    return SBG_NO_ERROR;
}

/*
 * Callback called when a full log is successfully received and parsed.
 */
SbgErrorCode Io_SbgEllipseGps_LogReceivedCallback(
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
                Io_SbgEllipseGps_ProcessImuMsg(msg_class, msg_id, log_data);
                break;
            }
            case SBG_ECOM_LOG_EKF_EULER:
            {
                Io_SbgEllipseGps_ProcessEulerAnglesMsg(msg_class, msg_id, log_data);
                break;
            }
            case SBG_ECOM_LOG_STATUS:
            {
                Io_SbgEllipseGps_ProcessStatusMsg(msg_class, msg_id, log_data);
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
static void Io_SbgEllipseGps_ProcessImuMsg(SbgEComClass msg_class, SbgEComMsgId msg, const SbgBinaryLogData *log_data)
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
    Io_SbgEllipseGps_ProcessEulerAnglesMsg(SbgEComClass msg_class, SbgEComMsgId msg, const SbgBinaryLogData *log_data)
{
    assert(msg_class == SBG_ECOM_CLASS_LOG_ECOM_0);
    assert(msg == SBG_ECOM_LOG_EKF_EULER);

    // Save euler angle data, in rad
    gps_data.euler_data.euler_angles.roll  = log_data->ekfEulerData.euler[0];
    gps_data.euler_data.euler_angles.pitch = log_data->ekfEulerData.euler[1];
    gps_data.euler_data.euler_angles.yaw   = log_data->ekfEulerData.euler[2];
}

/*
 * Process and save a new status msg.
 */
static void
    Io_SbgEllipseGps_ProcessStatusMsg(SbgEComClass msg_class, SbgEComMsgId msg, const SbgBinaryLogData *log_data)
{
    assert(msg_class == SBG_ECOM_CLASS_LOG_ECOM_0);
    assert(msg == SBG_ECOM_LOG_STATUS);

    // TODO
}

/* ------------------------- Public Function Definitions -------------------------- */

bool Io_SbgEllipseGps_Init()
{
    memset(&gps_data, 0, sizeof(GpsData));

    // Initialize the SBG serial interface handle
    Io_SbgEllipseGps_CreateSerialInterface(&sbg_interface);

    // Init SBG's communication protocol handle
    const SbgErrorCode com_init_err = sbgEComInit(&com_handle, &sbg_interface);

    if (com_init_err != SBG_NO_ERROR)
    {
        return false;
    }

    // Set the callback function (callback is called when a new log is successfully received and parsed)
    sbgEComSetReceiveLogCallback(&com_handle, Io_SbgEllipseGps_LogReceivedCallback, NULL);

    // Start waiting for UART packets
    Io_SbgEllipseGps_ReceiveUartPacket();

    return true;
}

void Io_SbgEllipseGps_Process()
{
    // Handle a single log. Calls the pReadFunc set in sbgInterfaceSerialCreate to get new data and attempts to parse
    // the log. If successful, the the receive log callback function set in init is triggered. If the log is incomplete,
    // the data will be saved to a buffer to be once more data is received.
    sbgEComHandleOneLog(&com_handle);

    // TODO: In interrupt, save to circular buffer
    // Change read function to read all from circular buffer
    // Call this in 100Hz or something
}

void Io_SbgEllipseGps_GetAttitude(Attitude *attitude)
{
    *attitude = gps_data.euler_data.euler_angles;
}
