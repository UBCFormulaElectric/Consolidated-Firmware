#include "torque_vectoring/estimation/dynamics_estimation.hpp"
#include "torque_vectoring/estimation/tire_model.hpp"

namespace app::tv
{
namespace
{
[[maybe_unused]] estimation::vehicleDynamics vehicle_dynamics_estimator{};

estimation::TireModel front_left_tire_model{
    estimation::TireModel::TirePressure::PSI_12,
    estimation::TireModel::WheelSide::Left,
    estimation::TireModel::WheelAxle::Front};

estimation::TireModel front_right_tire_model{
    estimation::TireModel::TirePressure::PSI_12,
    estimation::TireModel::WheelSide::Right,
    estimation::TireModel::WheelAxle::Front};

estimation::TireModel rear_left_tire_model{
    estimation::TireModel::TirePressure::PSI_12,
    estimation::TireModel::WheelSide::Left,
    estimation::TireModel::WheelAxle::Rear};

estimation::TireModel rear_right_tire_model{
    estimation::TireModel::TirePressure::PSI_12,
    estimation::TireModel::WheelSide::Right,
    estimation::TireModel::WheelAxle::Rear};
} // namespace

// optimzier
/*
// yaw rate control and pedal request



*/

} // namespace app::tv
