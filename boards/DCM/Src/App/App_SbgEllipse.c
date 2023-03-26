#include "App_SbgEllipse.h"
#include "App_CanTx.h"
#include "Io_SbgEllipseGps.h"

void App_SbgEllipse_Process()
{
    Io_SbgEllipseGps_Process();
}

void App_SbgEllipse_Broadcast()
{
    Attitude attitude;
    Io_SbgEllipseGps_GetAttitude(&attitude);

    App_CanTx_DCM_SbgEllipseSensor_Roll_Set(attitude.roll);
    App_CanTx_DCM_SbgEllipseSensor_Pitch_Set(attitude.pitch);
    App_CanTx_DCM_SbgEllipseSensor_Yaw_Set(attitude.yaw);
}
