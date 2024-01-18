#include "main.h"
#include <assert.h>
#include "sbgECom.h"
#include "interfaces/sbgInterfaceSerial.h"
#include "io_sbgEllipse.h"
#include "App_RingQueue.h"
#include "App_SharedMacros.h"
#include "FreeRTOS.h"

/* ------------------------------------ Defines ------------------------------------- */

#define UART_RX_PACKET_SIZE 128 // Size of each received UART packet, in bytes

/* ------------------------------------ Typedefs ------------------------------------- */

typedef struct
{
    float x;
    float y;
    float z;
} Vector3;

typedef struct
{
    float roll;
    float pitch;
    float yaw;
} Attitude;

typedef struct
{
    Vector3  acceleration;
    Attitude angular_velocity;
} ImuPacketData;

typedef struct
{
    Attitude euler_angles;
} EulerPacketData;

typedef struct
{
    uint32_t timestamp_us;
    uint16_t general_status;
    uint32_t com_status;
} StatusPacketData;

typedef struct
{
    ImuPacketData    imu_data;
    EulerPacketData  euler_data;
    StatusPacketData status_data;
} SensorData;

/* --------------------------------- Variables ---------------------------------- */

extern UART_HandleTypeDef huart1;

static SbgInterface  sbg_interface;                       // Handle for interface
static SbgEComHandle com_handle;                          // Handle for comms
static uint8_t       uart_rx_buffer[UART_RX_PACKET_SIZE]; // Buffer to hold last RXed UART packet
static RingQueue     rx_queue;                            // FIFO queue of RXed UART bytes
static SensorData    sensor_data;                         // Struct of all sensor data

// Map each sensor output enum to a ptr to the actual value
float *sensor_output_map[NUM_SBG_OUTPUTS] = {
    [SBG_ELLIPSE_OUT_ACCELERATION_X] = &sensor_data.imu_data.acceleration.x,
    [SBG_ELLIPSE_OUT_ACCELERATION_Y] = &sensor_data.imu_data.acceleration.y,
    [SBG_ELLIPSE_OUT_ACCELERATION_Z] = &sensor_data.imu_data.acceleration.z,

    [SBG_ELLIPSE_OUT_ANGULAR_VELOCITY_ROLL]  = &sensor_data.imu_data.angular_velocity.roll,
    [SBG_ELLIPSE_OUT_ANGULAR_VELOCITY_PITCH] = &sensor_data.imu_data.angular_velocity.pitch,
    [SBG_ELLIPSE_OUT_ANGULAR_VELOCITY_YAW]   = &sensor_data.imu_data.angular_velocity.yaw,

    [SBG_ELLIPSE_OUT_EULER_ROLL]  = &sensor_data.euler_data.euler_angles.roll,
    [SBG_ELLIPSE_OUT_EULER_PITCH] = &sensor_data.euler_data.euler_angles.pitch,
    [SBG_ELLIPSE_OUT_EULER_YAW]   = &sensor_data.euler_data.euler_angles.yaw,
};

/* ------------------------- Static Function Prototypes -------------------------- */

static void         io_sbgEllipse_createSerialInterface(SbgInterface *interface);
static SbgErrorCode io_sbgEllipse_read(SbgInterface *interface, void *buffer, size_t *read_bytes, size_t bytes_to_read);
static SbgErrorCode io_sbgEllipse_logReceivedCallback(
    SbgEComHandle *         handle,
    SbgEComClass            msg_class,
    SbgEComMsgId            msg_id,
    const SbgBinaryLogData *log_data,
    void *                  user_arg);
static void io_sbgEllipse_processMsg_imu(const SbgBinaryLogData *log_data);
static void io_sbgEllipse_processMsg_eulerAngles(const SbgBinaryLogData *log_data);
static void io_sbgEllipse_processMsg_status(const SbgBinaryLogData *log_data);

/* ------------------------- Static Function Definitions -------------------------- */

/*
 * Callback called when a UART packet is received.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // NOTE: I previously tried handling logs within the interrupt itself, but this was throwing errors.
    // Not sure why but the error msg was related to mallocing within an ISR, although I couldn't figure out where in
    // SBG's library anything was actually being malloced. This is why I push to a queue here and handle the logs later
    // in the 100Hz task.

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
 * to the sensor. We only need read operations, so that's all we provide here.
 */
static void io_sbgEllipse_createSerialInterface(SbgInterface *interface)
{
    sbgInterfaceNameSet(interface, "SBG Ellipse N Sensor");

    interface->type          = SBG_IF_TYPE_UNKNOW;
    interface->pDestroyFunc  = NULL;
    interface->pWriteFunc    = NULL;
    interface->pReadFunc     = io_sbgEllipse_read;
    interface->pFlushFunc    = NULL;
    interface->pSetSpeedFunc = NULL;
    interface->pGetSpeedFunc = NULL;
    interface->pDelayFunc    = NULL;
}

/*
 * Function called by SBG's library to read some amount of data.
 */
static SbgErrorCode io_sbgEllipse_read(SbgInterface *interface, void *buffer, size_t *read_bytes, size_t bytes_to_read)
{
    UNUSED(interface);

    // Disable interrupts so UART RX callback won't push data to the queue while we're reading from it
    vPortEnterCritical();
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
        (*read_bytes)++;
        i++;
    }

    vPortExitCritical();
    return SBG_NO_ERROR;
}

/*
 * Callback called when a log is successfully received and parsed.
 */
SbgErrorCode io_sbgEllipse_logReceivedCallback(
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
                io_sbgEllipse_processMsg_imu(log_data);
                break;
            }
            case SBG_ECOM_LOG_EKF_EULER:
            {
                io_sbgEllipse_processMsg_eulerAngles(log_data);
                break;
            }
            case SBG_ECOM_LOG_STATUS:
            {
                io_sbgEllipse_processMsg_status(log_data);
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
static void io_sbgEllipse_processMsg_imu(const SbgBinaryLogData *log_data)
{
    // Save acceleration, in m/s^2
    sensor_data.imu_data.acceleration.x = log_data->imuData.accelerometers[0];
    sensor_data.imu_data.acceleration.y = log_data->imuData.accelerometers[1];
    sensor_data.imu_data.acceleration.z = log_data->imuData.accelerometers[2];

    // Save angular velocity, in deg/s
    sensor_data.imu_data.angular_velocity.roll  = RAD_TO_DEG(log_data->imuData.gyroscopes[0]);
    sensor_data.imu_data.angular_velocity.pitch = RAD_TO_DEG(log_data->imuData.gyroscopes[1]);
    sensor_data.imu_data.angular_velocity.yaw   = RAD_TO_DEG(log_data->imuData.gyroscopes[2]);
}

/*
 * Process and save a new euler angles msg.
 */
static void io_sbgEllipse_processMsg_eulerAngles(const SbgBinaryLogData *log_data)
{
    // Save euler angles, in deg
    sensor_data.euler_data.euler_angles.roll  = RAD_TO_DEG(log_data->ekfEulerData.euler[0]);
    sensor_data.euler_data.euler_angles.pitch = RAD_TO_DEG(log_data->ekfEulerData.euler[1]);
    sensor_data.euler_data.euler_angles.yaw   = RAD_TO_DEG(log_data->ekfEulerData.euler[2]);
}

/*
 * Process and save a new status msg.
 */
static void io_sbgEllipse_processMsg_status(const SbgBinaryLogData *log_data)
{
    sensor_data.status_data.timestamp_us   = log_data->statusData.timeStamp;
    sensor_data.status_data.general_status = log_data->statusData.generalStatus;
    sensor_data.status_data.com_status     = log_data->statusData.comStatus;
}

/* ------------------------- Public Function Definitions -------------------------- */

bool io_sbgEllipse_init()
{
    memset(&sensor_data, 0, sizeof(SensorData));

    // Initialize the SBG serial interface handle
    io_sbgEllipse_createSerialInterface(&sbg_interface);

    // Init SBG's communication protocol handle
    if (sbgEComInit(&com_handle, &sbg_interface) != SBG_NO_ERROR)
    {
        return false;
    }

    // Set the callback function (callback is called when a new log is successfully received and parsed)
    sbgEComSetReceiveLogCallback(&com_handle, io_sbgEllipse_logReceivedCallback, NULL);

    // Init RX queue for UART data
    App_RingQueue_Init(&rx_queue, RING_QUEUE_MAX_SIZE);

    // Start waiting for UART packets
    HAL_UART_Receive_DMA(&huart1, uart_rx_buffer, UART_RX_PACKET_SIZE);

    return true;
}

void io_sbgEllipse_handleLogs()
{
    // Handle logs. Calls the pReadFunc set in sbgInterfaceSerialCreate to read data and parses
    // all logs found in the data. Upon successfully parsing a log, the the receive log callback function set in init is
    // triggered. Incomplete log data will be saved to a buffer in SBG's library to be used once more data is received.
    sbgEComHandle(&com_handle);
}

uint32_t io_sbgEllipse_getTimestampUs(void)
{
    return sensor_data.status_data.timestamp_us;
}

float io_sbgEllipse_getSensorOutput(SbgEllipseOutput output)
{
    assert(output < NUM_SBG_OUTPUTS);
    return *(sensor_output_map[output]);
}

uint16_t io_sbgEllipse_getGeneralStatus()
{
    return sensor_data.status_data.general_status;
}

uint32_t io_sbgEllipse_getComStatus()
{
    return sensor_data.status_data.com_status;
}
