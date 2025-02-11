#include "io_sbgEllipse.h"

// message queues
#include "cmsis_os.h"
#include <assert.h>

// sbg
#include "interfaces/sbgInterfaceSerial.h"
#include "sbgECom.h"

#include "app_units.h"
#include "io_log.h"
#include "hw_uarts.h"

/* ------------------------- Static Function Prototypes -------------------------- */
/** Callbacks */
static SbgErrorCode pReadFunc(SbgInterface *interface, void *buffer, size_t *read_bytes, size_t bytes_to_read);
static SbgErrorCode pReceiveLogCallback(
    SbgEComHandle          *handle,
    SbgEComClass            msg_class,
    SbgEComMsgId            msg_id,
    const SbgBinaryLogData *log_data,
    void                   *user_arg);

/** Processing Functions */
static void processMsg_Imu(const SbgBinaryLogData *log_data);
static void processMsg_eulerAngles(const SbgBinaryLogData *log_data);
static void processMsg_status(const SbgBinaryLogData *log_data);
static void processMsg_EkfNavVelandPos(const SbgBinaryLogData *log_data);

/* --------------------------------- Variables ---------------------------------- */
// SBG Interface Variables
static SbgInterface  sbg_interface = { .handle        = NULL,
                                       .type          = SBG_IF_TYPE_UNKNOW,
                                       .pDestroyFunc  = NULL,
                                       .pWriteFunc    = NULL,
                                       .pReadFunc     = pReadFunc,
                                       .pFlushFunc    = NULL,
                                       .pSetSpeedFunc = NULL,
                                       .pGetSpeedFunc = NULL,
                                       .pDelayFunc    = NULL };
static SbgEComHandle com_handle; // Handle for comms

/** UART Internals */
#define UART_RX_PACKET_SIZE 128                     // Size of each received UART packet, in bytes
static uint8_t uart_rx_buffer[UART_RX_PACKET_SIZE]; // Buffer to hold last RXed UART packet

/** Message Queue Variables */
// NOTE: make sure below is a even factor, by tuning UART_RX_PACKET_SIZE
#define QUEUE_MAX_SIZE (SBG_ECOM_MAX_BUFFER_SIZE / UART_RX_PACKET_SIZE) // max amt of elements in queue
static osMessageQueueId_t         sensor_rx_queue_id;
static StaticQueue_t              rx_queue_control_block;
static uint8_t                    sensor_rx_queue_buf[SBG_ECOM_MAX_BUFFER_SIZE];
static const osMessageQueueAttr_t sensor_rx_queue_attr = {
    .name      = "SensorRxQueue",
    .attr_bits = 0,
    .cb_mem    = &rx_queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = sensor_rx_queue_buf,
    .mq_size   = SBG_ECOM_MAX_BUFFER_SIZE,
};
static uint32_t sbg_queue_overflow_count;

/** Struct of all sensor data */
static SensorData sensor_data;

/* ------------------------- Static Function Definitions -------------------------- */
/*
 * Function called by SBG's library to read some amount of data.
 */
// ReSharper disable once CppParameterMayBeConstPtrOrRef
static SbgErrorCode pReadFunc(SbgInterface *interface, void *buffer, size_t *read_bytes, const size_t bytes_to_read)
{
    assert(interface == &sbg_interface);

    // TODO double check if we need this? osMessageQueue should automatically prevent this
    // also not entirely sure why we need this in the first place? How does this get called?
    // Disable interrupts so UART RX callback won't push data to the queue while we're reading from it
    vPortEnterCritical();
    *read_bytes = 0;
    // Read all available data from the RX queue, up to the requested amount
    while (*read_bytes < bytes_to_read)
    {
        if (osMessageQueueGetCount(sensor_rx_queue_id) == 0)
            break;

        // copy from queue into
        uint8_t packet[UART_RX_PACKET_SIZE];
        if (osMessageQueueGet(sensor_rx_queue_id, packet, NULL, osWaitForever) != osOK)
            break;

        // TODO does this discard bytes?? It might get popped off the queue and not copied??
        assert(bytes_to_read % UART_RX_PACKET_SIZE == 0);
        const size_t bytes_to_copy = MIN(bytes_to_read - *read_bytes, UART_RX_PACKET_SIZE);
        memcpy((uint8_t *)buffer + *read_bytes, packet, bytes_to_copy);
        *read_bytes += bytes_to_copy;
    }
    vPortExitCritical();
    return SBG_NO_ERROR;
}

/*
 * Callback called when a log is successfully received and parsed.
 * Dispatches one of the processing functions to process data into sensor_data
 */
SbgErrorCode pReceiveLogCallback(
    // ReSharper disable once CppParameterMayBeConstPtrOrRef
    SbgEComHandle          *handle,
    const SbgEComClass      msg_class,
    const SbgEComMsgId      msg_id,
    const SbgBinaryLogData *log_data,
    // ReSharper disable once CppParameterMayBeConstPtrOrRef
    void *user_arg)
{
    assert(log_data);
    assert(handle == &com_handle);
    UNUSED(user_arg);

    if (msg_class == SBG_ECOM_CLASS_LOG_ECOM_0)
    {
        switch (msg_id)
        {
            case SBG_ECOM_LOG_IMU_DATA:
            {
                processMsg_Imu(log_data);
                break;
            }
            case SBG_ECOM_LOG_EKF_EULER:
            {
                processMsg_eulerAngles(log_data);
                break;
            }
            case SBG_ECOM_LOG_STATUS:
            {
                processMsg_status(log_data);
                break;
            }
            case SBG_ECOM_LOG_EKF_NAV:
            {
                processMsg_EkfNavVelandPos(log_data);
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
static void processMsg_Imu(const SbgBinaryLogData *log_data)
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
static void processMsg_eulerAngles(const SbgBinaryLogData *log_data)
{
    // Save euler angles, in deg
    sensor_data.ekf_euler_data.euler_angles.roll  = RAD_TO_DEG(log_data->ekfEulerData.euler[0]);
    sensor_data.ekf_euler_data.euler_angles.pitch = RAD_TO_DEG(log_data->ekfEulerData.euler[1]);
    sensor_data.ekf_euler_data.euler_angles.yaw   = RAD_TO_DEG(log_data->ekfEulerData.euler[2]);
}

/*
 * Process and save a new status msg.
 */
static void processMsg_status(const SbgBinaryLogData *log_data)
{
    sensor_data.status_data.timestamp_us   = log_data->statusData.timeStamp;
    sensor_data.status_data.general_status = log_data->statusData.generalStatus;
    sensor_data.status_data.com_status     = log_data->statusData.comStatus;
}

/*
 * Process and save relevant EKF Navigation Velocity and Position information.
 */
static void processMsg_EkfNavVelandPos(const SbgBinaryLogData *log_data)
{
    // TODO: uncomment after initial testing, if this occurs skip reading data

    // obtaining ekf solution mode from sbg ellipse and setting it in status field
    sensor_data.ekf_solution_status = sbgEComLogEkfGetSolutionMode(log_data->ekfNavData.status);

    // velocity data in m/s
    sensor_data.ekf_nav_data.velocity.north = log_data->ekfNavData.velocity[0];
    sensor_data.ekf_nav_data.velocity.east  = log_data->ekfNavData.velocity[1];
    sensor_data.ekf_nav_data.velocity.down  = log_data->ekfNavData.velocity[2];

    // velocity standard dev
    sensor_data.ekf_nav_data.velocity.north_std_dev = log_data->ekfNavData.velocityStdDev[0];
    sensor_data.ekf_nav_data.velocity.east_std_dev  = log_data->ekfNavData.velocityStdDev[1];
    sensor_data.ekf_nav_data.velocity.down_std_dev  = log_data->ekfNavData.velocityStdDev[2];

    // position data in m
    sensor_data.ekf_nav_data.position.latitude  = log_data->ekfNavData.position[0];
    sensor_data.ekf_nav_data.position.longitude = log_data->ekfNavData.position[1];
    sensor_data.ekf_nav_data.position.altitude  = log_data->ekfNavData.position[2];

    // position standard dev
    sensor_data.ekf_nav_data.position.altitude_std_dev  = log_data->ekfNavData.positionStdDev[0];
    sensor_data.ekf_nav_data.position.latitude_std_dev  = log_data->ekfNavData.positionStdDev[1];
    sensor_data.ekf_nav_data.position.longitude_std_dev = log_data->ekfNavData.positionStdDev[2];
}

/* ------------------------- Public Function Definitions -------------------------- */

bool io_sbgEllipse_init()
{
    // 1. Initialize sbg_interface
    sbgInterfaceNameSet(&sbg_interface, "SBG Ellipse N Sensor"); // Dynamic initialization of the name

    // 2. Initialize the SBG serial interface handle
    const SbgErrorCode sbgEComInitCode = sbgEComInit(&com_handle, &sbg_interface);
    // Init SBG's communication protocol handle
    if (sbgEComInitCode != SBG_NO_ERROR)
    {
        LOG_INFO("%d", sbgEComInitCode);
        return false;
    }
    // Set the callback function (callback is called when a new log is successfully received and parsed)
    sbgEComSetReceiveLogCallback(&com_handle, pReceiveLogCallback, NULL);

    // Init RX queue for UART data
    sensor_rx_queue_id = osMessageQueueNew(QUEUE_MAX_SIZE, UART_RX_PACKET_SIZE, &sensor_rx_queue_attr);
    assert(sensor_rx_queue_id != NULL);

    // Start waiting for UART packets
    hw_uart_receiveDma(&sbg_uart, uart_rx_buffer, UART_RX_PACKET_SIZE);

    return true;
}

void io_sbgEllipse_handleLogs(void)
{
    // Handle logs. CALLS THE pReadFunc set in sbgInterfaceSerialCreate to read data and parses
    // all logs found in the data. Upon successfully parsing a log, the the receive log callback function set in init is
    // triggered. Incomplete log data will be saved to a buffer in SBG's library to be used once more data is received.
    const SbgErrorCode errorCode = sbgEComHandle(&com_handle);
    if (errorCode != SBG_NO_ERROR)
    {
        // handle error
        char buffer[256];
        sbgEComErrorToString(errorCode, buffer);
        LOG_INFO("%s", buffer);
    }
}

uint32_t io_sbgEllipse_getTimestampUs(void)
{
    return sensor_data.status_data.timestamp_us;
}

uint16_t io_sbgEllipse_getGeneralStatus(void)
{
    return sensor_data.status_data.general_status;
}

uint32_t io_sbgEllipse_getComStatus(void)
{
    return sensor_data.status_data.com_status;
}

uint32_t io_sbgEllipse_getOverflowCount(void)
{
    return sbg_queue_overflow_count;
}

uint32_t io_sbgEllipse_getEkfSolutionMode(void)
{
    return sensor_data.ekf_solution_status;
}

Vector3 *io_sbgEllipse_getImuAccelerations()
{
    return &sensor_data.imu_data.acceleration;
}

Attitude *io_sbgEllipse_getImuAngularVelocities()
{
    return &sensor_data.imu_data.angular_velocity;
}

Attitude *io_sbgEllipse_getEkfEulerAngles()
{
    return &sensor_data.ekf_euler_data.euler_angles;
}

VelocityData *io_sbgEllipse_getEkfNavVelocityData()
{
    return &sensor_data.ekf_nav_data.velocity;
}

PositionData *io_sbgEllipse_getEkfNavPositionData()
{
    return &sensor_data.ekf_nav_data.position;
}

void io_sbgEllipse_msgRxCallback(void)
{
    sbg_queue_overflow_count = 0;
    if (osMessageQueuePut(sensor_rx_queue_id, &uart_rx_buffer, 0, 0) != osOK)
    {
        sbg_queue_overflow_count++;
    }
    LOG_INFO("%d", sbg_queue_overflow_count);
}
