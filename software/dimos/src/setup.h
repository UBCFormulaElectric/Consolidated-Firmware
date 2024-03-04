#pragma once

#include "shared/types.h"
#include "jsoncanqml.h"
#include <QtQml>

void set_qt_environment();
void init_json_can();


enum class CAN_setup_errors
{
};
static const std::map<CAN_setup_errors, std::string> CAN_setup_errors_str = {};
static JSONCANQML                                    jsoncan_qml_interface;
Result<std::monostate, CAN_setup_errors> setupCanThreads(const QQmlApplicationEngine *engine_ref);


enum class GPIO_setup_errors
{
    LINE_SETUP_ERROR
};
const std::map<GPIO_setup_errors, std::string> GPIO_setup_errors_str = { { GPIO_setup_errors::LINE_SETUP_ERROR,
                                                                           "Line Setup Error" } };
Result<std::monostate, GPIO_setup_errors> setupGPIOThreads(const QQmlApplicationEngine *engine_ref);
