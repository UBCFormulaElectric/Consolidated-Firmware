#include <algorithm>
#include "app_imu.hpp"
#include "app_canAlerts.hpp"
#include "app_canTx.hpp"
#include "io_imu.hpp"

namespace app::imus
{
using namespace app::can_alerts;

static VCImuResults                   imu_results  = { .accel_x_res = std::unexpected(ErrorCode::ERROR),
                                                       .accel_y_res = std::unexpected(ErrorCode::ERROR),
                                                       .accel_z_res = std::unexpected(ErrorCode::ERROR),
                                                       .gyro_x_res  = std::unexpected(ErrorCode::ERROR),
                                                       .gyro_y_res  = std::unexpected(ErrorCode::ERROR),
                                                       .gyro_z_res  = std::unexpected(ErrorCode::ERROR) };
static std::expected<void, ErrorCode> imu1_init_ok = std::unexpected(ErrorCode::ERROR);
static std::expected<void, ErrorCode> imu2_init_ok = std::unexpected(ErrorCode::ERROR);
static std::expected<void, ErrorCode> imu3_init_ok = std::unexpected(ErrorCode::ERROR);

struct AxisData
{
    std::expected<float, ErrorCode> x = std::unexpected(ErrorCode::ERROR);
    std::expected<float, ErrorCode> y = std::unexpected(ErrorCode::ERROR);
    std::expected<float, ErrorCode> z = std::unexpected(ErrorCode::ERROR);
};

// Returns the median of three valid readings.
// If one reading is invalid, returns the average of the two valid readings.
// If two readings are invalid, returns the remaining valid reading.
// If all readings are invalid, returns the first error observed.
static inline std::expected<float, ErrorCode> median3(
    const std::expected<float, ErrorCode> &a,
    const std::expected<float, ErrorCode> &b,
    const std::expected<float, ErrorCode> &c)
{
    const auto average = [](const float x, const float y) { return 0.5f * (x + y); };

    const auto first_error = [&]() -> ErrorCode
    {
        if (not a)
            return a.error();
        if (not b)
            return b.error();
        if (not c)
            return c.error();
        return ErrorCode::ERROR_INDETERMINATE;
    };

    if (a && b && c)
    {
        return a.value() + b.value() + c.value() - std::min({ a.value(), b.value(), c.value() }) -
               std::max({ a.value(), b.value(), c.value() });
    }

    if (a && b)
        return average(a.value(), b.value());
    if (a && c)
        return average(a.value(), c.value());
    if (b && c)
        return average(b.value(), c.value());

    if (a)
        return a.value();
    if (b)
        return b.value();
    if (c)
        return c.value();

    return std::unexpected(first_error());
}

template <typename T> static AxisData splitAxes(const std::expected<T, ErrorCode> &data)
{
    if (!data)
    {
        const auto err = data.error();
        return { std::unexpected(err), std::unexpected(err), std::unexpected(err) };
    }

    return { data.value().x, data.value().y, data.value().z };
}

static AxisData medianAxes(const AxisData &a, const AxisData &b, const AxisData &c)
{
    return {
        .x = median3(a.x, b.x, c.x),
        .y = median3(a.y, b.y, c.y),
        .z = median3(a.z, b.z, c.z),
    };
}

void init()
{
    imu1_init_ok = IMU1.init();
    imu2_init_ok = IMU2.init();
    imu3_init_ok = IMU3.init();

    warnings::Imu1InitFailed_set(not imu1_init_ok.has_value());
    warnings::Imu2InitFailed_set(not imu2_init_ok.has_value());
    warnings::Imu3InitFailed_set(not imu3_init_ok.has_value());
}

void broadcast()
{
    // IMU1 data broadcasting
    const auto accel1 = IMU1.getAccelAll();
    const auto gyro1  = IMU1.getGyroAll();
    // IMU2 data broadcasting
    const auto accel2 = IMU2.getAccelAll();
    const auto gyro2  = IMU2.getGyroAll();
    // IMU3 data broadcasting
    const auto accel3 = IMU3.getAccelAll();
    const auto gyro3  = IMU3.getGyroAll();

    // If either accel or gyro fails for an IMU, set a fault alert
    if (!accel1 || !gyro1)
        infos::Imu1Fault_set(true);

    if (!accel2 || !gyro2)
        infos::Imu2Fault_set(true);

    if (!accel3 || !gyro3)
        infos::Imu3Fault_set(true);

    const auto accel1_data    = accel1.value_or(io::Imu::AccelData{ 0.0f, 0.0f, 0.0f });
    const auto accel2_data    = accel2.value_or(io::Imu::AccelData{ 0.0f, 0.0f, 0.0f });
    const auto accel3_data    = accel3.value_or(io::Imu::AccelData{ 0.0f, 0.0f, 0.0f });
    const auto gyro1_data     = gyro1.value_or(io::Imu::GyroData{ 0.0f, 0.0f, 0.0f });
    const auto gyro2_data     = gyro2.value_or(io::Imu::GyroData{ 0.0f, 0.0f, 0.0f });
    const auto gyro3_data     = gyro3.value_or(io::Imu::GyroData{ 0.0f, 0.0f, 0.0f });
    const auto accel_filtered = medianAxes(splitAxes(accel1), splitAxes(accel2), splitAxes(accel3));
    const auto gyro_filtered  = medianAxes(splitAxes(gyro1), splitAxes(gyro2), splitAxes(gyro3));

    imu_results = { .accel_x_res = accel_filtered.x,
                    .accel_y_res = accel_filtered.y,
                    .accel_z_res = accel_filtered.z,
                    .gyro_x_res  = gyro_filtered.x,
                    .gyro_y_res  = gyro_filtered.y,
                    .gyro_z_res  = gyro_filtered.z };

    app::can_tx::VC_Imu1AccelX_set(accel1_data.x);
    app::can_tx::VC_Imu1AccelY_set(accel1_data.y);
    app::can_tx::VC_Imu1AccelZ_set(accel1_data.z);
    app::can_tx::VC_Imu1GyroX_set(gyro1_data.x);
    app::can_tx::VC_Imu1GyroY_set(gyro1_data.y);
    app::can_tx::VC_Imu1GyroZ_set(gyro1_data.z);

    app::can_tx::VC_Imu2AccelX_set(accel2_data.x);
    app::can_tx::VC_Imu2AccelY_set(accel2_data.y);
    app::can_tx::VC_Imu2AccelZ_set(accel2_data.z);
    app::can_tx::VC_Imu2GyroX_set(gyro2_data.x);
    app::can_tx::VC_Imu2GyroY_set(gyro2_data.y);
    app::can_tx::VC_Imu2GyroZ_set(gyro2_data.z);

    app::can_tx::VC_Imu3AccelX_set(accel3_data.x);
    app::can_tx::VC_Imu3AccelY_set(accel3_data.y);
    app::can_tx::VC_Imu3AccelZ_set(accel3_data.z);
    app::can_tx::VC_Imu3GyroX_set(gyro3_data.x);
    app::can_tx::VC_Imu3GyroY_set(gyro3_data.y);
    app::can_tx::VC_Imu3GyroZ_set(gyro3_data.z);

    app::can_tx::VC_ImuFilteredAccelX_set(accel_filtered.x.value_or(0.0f));
    app::can_tx::VC_ImuFilteredAccelY_set(accel_filtered.y.value_or(0.0f));
    app::can_tx::VC_ImuFilteredAccelZ_set(accel_filtered.z.value_or(0.0f));
    app::can_tx::VC_ImuFilteredGyroX_set(gyro_filtered.x.value_or(0.0f));
    app::can_tx::VC_ImuFilteredGyroY_set(gyro_filtered.y.value_or(0.0f));
    app::can_tx::VC_ImuFilteredGyroZ_set(gyro_filtered.z.value_or(0.0f));
}

std::expected<float, ErrorCode> getAccelX()
{
    return imu_results.accel_x_res;
}

std::expected<float, ErrorCode> getAccelY()
{
    return imu_results.accel_y_res;
}

std::expected<float, ErrorCode> getAccelZ()
{
    return imu_results.accel_z_res;
}

std::expected<float, ErrorCode> getGyroX()
{
    return imu_results.gyro_x_res;
}

std::expected<float, ErrorCode> getGyroY()
{
    return imu_results.gyro_y_res;
}

std::expected<float, ErrorCode> getGyroZ()
{
    return imu_results.gyro_z_res;
}

bool getAllInitsFailed()
{
    return not imu1_init_ok.has_value() && not imu2_init_ok.has_value() && not imu3_init_ok.has_value();
}

// TODO: state estimation for which IMU is faulty if any and maybe a way to determine if the IMU is giving bad data but
// not fully broken (ex: values are all 0 or not changing at all) and then use the good IMUs to estimate what the bad
// IMU should be outputting and broadcast that instead, and set a fault alert for that IMU as well. Could also use this
// state estimation to determine which IMU data to trust if there is disagreement between the 3 IMUs (ex: 2 similar
// outputs and 1 different output, then take the 2 similar outputs as correct and set a fault for the different one).
} // namespace app::imus
