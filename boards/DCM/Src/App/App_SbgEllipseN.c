#include "App_SbgEllipseN.h"
#include "App_CanTx.h"
#include "Io_SbgEllipseN.h"

void App_SbgEllipseN_HandleLogs()
{
    Io_SbgEllipseN_HandleLogs();
}

void App_SbgEllipseN_Broadcast()
{
    Attitude attitude;
    Io_SbgEllipseN_GetAttitude(&attitude);

    App_CanTx_DCM_SbgEllipseSensor_Roll_Set(attitude.roll);
    App_CanTx_DCM_SbgEllipseSensor_Pitch_Set(attitude.pitch);
    App_CanTx_DCM_SbgEllipseSensor_Yaw_Set(attitude.yaw);
}
