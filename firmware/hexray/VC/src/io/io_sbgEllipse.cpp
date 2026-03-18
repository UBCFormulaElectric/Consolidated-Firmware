#include "io_sbgEllipse.hpp"

extern "C"
{
#include "sbgECom.h"
#include "FreeRTOS.h"
#include "stream_buffer.h"
}

#include "hw_uarts.hpp"
#include "util_units.hpp"

constexpr int UART_RX_PACKET_SIZE = 128; // Size of each received UART packet, in bytes
constexpr int QUEUE_MAX_SIZE      = 32;  // 128 * 32 = 4096 which is SBG_ECOM_MAX_BUFFER_SIZE

namespace io::sbgEllipse
{

static SbgInterface  abg_interface;                     // Handle for interface (abgs gon want us w this handle)
static SbgEComHandle com_handle;                        // Handle for comms
static uint8_t       uart_dma_buf[UART_RX_PACKET_SIZE]; // Buffer to hold last RXed UART packet
static SensorData    sensor_data;                       // Struct of all sensor data

static StaticStreamBuffer_t uart_sbuf_ctrl;
static uint8_t              uart_sbuf_data[QUEUE_MAX_SIZE * UART_RX_PACKET_SIZE];
static StreamBufferHandle_t uart_sbuf_handle;
static uint8_t              sbg_queue_overflow_count;

static void         createSerialInterface(SbgInterface *interface);
static SbgErrorCode read(SbgInterface *interface, void *buffer, size_t *read_bytes, size_t bytes_to_read);
static SbgErrorCode logReceivedCallback(
    SbgEComHandle         *handle,
    SbgEComClass           msg_class,
    SbgEComMsgId           msg_id,
    const SbgEComLogUnion *log_data,
    void                  *user_arg);

static void processMsg_Imu(const SbgEComLogUnion *log_data);
static void processMsg_eulerAngles(const SbgEComLogUnion *log_data);
static void processMsg_status(const SbgEComLogUnion *log_data);
static void processMsg_EkfNavVelandPos(const SbgEComLogUnion *log_data);

/*
 * Callback called when a UART packet is received.
 */

/*
 * Create a serial interface that will be used by SBG's library. Requires passing pointers to functions for all I/O
 * operations, such as reading, writing, flushing, etc. These I/O operations will use STM32's HAL drivers to communicate
 * to the sensor. We only need read operations, so that's all we provide here.
 */
static void createSerialInterface(SbgInterface *interface)
{
    sbgInterfaceNameSet(interface, "SBG Ellipse N Sensor");

    interface->type          = SBG_IF_TYPE_UNKNOW;
    interface->pDestroyFunc  = NULL;
    interface->pWriteFunc    = NULL;
    interface->pReadFunc     = read;
    interface->pFlushFunc    = NULL;
    interface->pSetSpeedFunc = NULL;
    interface->pGetSpeedFunc = NULL;
    interface->pDelayFunc    = NULL;
}

/*
 * Function called by SBG's library to read some amount of data.
 */
static SbgErrorCode read(SbgInterface *interface, void *buffer, size_t *read_bytes, size_t bytes_to_read)
{
    UNUSED(interface);

    // Read all available data from the RX queue, up to the requested amount
    *read_bytes = xStreamBufferReceive(uart_sbuf_handle, buffer, bytes_to_read, 0U);

    return SBG_NO_ERROR;
}

/*
 * Callback called when a log is successfully received and parsed.
 */
SbgErrorCode logReceivedCallback(
    SbgEComHandle         *handle,
    SbgEComClass           msg_class,
    SbgEComMsgId           msg_id,
    const SbgEComLogUnion *log_data,
    void                  *user_arg)
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
static void processMsg_Imu(const SbgEComLogUnion *log_data)
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
static void processMsg_eulerAngles(const SbgEComLogUnion *log_data)
{
    // Save euler angles, in deg
    sensor_data.ekf_euler_data.euler_angles.roll  = RAD_TO_DEG(log_data->ekfEulerData.euler[0]);
    sensor_data.ekf_euler_data.euler_angles.pitch = RAD_TO_DEG(log_data->ekfEulerData.euler[1]);
    sensor_data.ekf_euler_data.euler_angles.yaw   = RAD_TO_DEG(log_data->ekfEulerData.euler[2]);
}

/*
 * Process and save a new status msg.
 */
static void processMsg_status(const SbgEComLogUnion *log_data)
{
    sensor_data.status_data.timestamp_us   = log_data->statusData.timeStamp;
    sensor_data.status_data.general_status = log_data->statusData.generalStatus;
    sensor_data.status_data.com_status     = log_data->statusData.comStatus;
}

/*
 * Process and save relevant EKF Navigation Velocity and Position information.
 */
static void processMsg_EkfNavVelandPos(const SbgEComLogUnion *log_data)
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

std::expected<void, ErrorCode> init()
{
    memset(&sensor_data, 0, sizeof(SensorData));

    // Initialize the SBG serial interface handle
    createSerialInterface(&abg_interface);

    SbgErrorCode sbgEComInitCode = sbgEComInit(&com_handle, &abg_interface);

    // Init SBG's communication protocol handle
    if (sbgEComInitCode != SBG_NO_ERROR)
    {
        LOG_INFO("%d", sbgEComInitCode);
        return std::unexpected(ErrorCode::ERROR);
    }

    // Set the callback function (callback is called when a new log is successfully received and parsed)
    sbgEComSetReceiveLogCallback(&com_handle, logReceivedCallback, NULL);

    // Init stream buffer for UART data
    uart_sbuf_handle = xStreamBufferCreateStatic(sizeof(uart_sbuf_data), 1, uart_sbuf_data, &uart_sbuf_ctrl);
    assert(uart_sbuf_handle != NULL);

    // Start waiting for UART packets
    RETURN_IF_ERR(sbg_ellipse_uart.receiveDma(std::span<uint8_t>{ uart_dma_buf }));
    return {};
}

void handleLogs(void)
{
    // Handle logs. Calls the pReadFunc set in sbgInterfaceSerialCreate to read data and parses
    // all logs found in the data. Upon successfully parsing a log, the the receive log callback function set in init is
    // triggered. Incomplete log data will be saved to a buffer in SBG's library to be used once more data is received.

    SbgErrorCode error_code = SBG_NO_ERROR;
    while (error_code != SBG_NOT_READY) // This is returned when no more logs could be parsed
    {
        error_code = sbgEComHandleOneLog(&com_handle);

        if (error_code != SBG_NO_ERROR && error_code != SBG_NOT_READY)
        {
            char buffer[256];
            sbgEComErrorToString(error_code, buffer);
            LOG_INFO("SBG Ellipse logging error: %s", buffer);
            // app::can_tx::VC_MSDOrEMeterOKStatus_set((ellipseErrorCode)error_code);
        }
    }
}

uint32_t getTimestampUs(void)
{
    return sensor_data.status_data.timestamp_us;
}

uint16_t getGeneralStatus(void)
{
    return sensor_data.status_data.general_status;
}

uint32_t getComStatus(void)
{
    return sensor_data.status_data.com_status;
}

uint8_t getOverflowCount(void)
{
    return sbg_queue_overflow_count;
}

uint32_t getEkfSolutionMode(void)
{
    return sensor_data.ekf_solution_status;
}

Vector3 *getImuAccelerations(void)
{
    return &sensor_data.imu_data.acceleration;
}

Attitude *getImuAngularVelocities(void)
{
    return &sensor_data.imu_data.angular_velocity;
}

Attitude *getEkfEulerAngles(void)
{
    return &sensor_data.ekf_euler_data.euler_angles;
}

VelocityData *getEkfNavVelocityData(void)
{
    return &sensor_data.ekf_nav_data.velocity;
}

PositionData *getEkfNavPositionData(void)
{
    return &sensor_data.ekf_nav_data.position;
}

void msgRxCallback(void)
{
    BaseType_t higher_priority_task_woken;
    if (xStreamBufferSendFromISR(uart_sbuf_handle, uart_dma_buf, sizeof(uart_dma_buf), &higher_priority_task_woken) <
        sizeof(uart_dma_buf))
    {
        sbg_queue_overflow_count++;
        LOG_WARN("SBG Ellipse IMU queue overflow: %d", sbg_queue_overflow_count);
    }

    portYIELD_FROM_ISR(higher_priority_task_woken);
}

} // namespace io::sbgEllipse
