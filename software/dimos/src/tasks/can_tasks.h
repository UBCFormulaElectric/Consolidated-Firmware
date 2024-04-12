#pragma once
#include <QThread>
#include <QMutex>

#include "ui/canqml/canqml.h"
#include "types.h"

namespace CanTask
{
enum class CAN_setup_errors
{
};
static const std::map<CAN_setup_errors, std::string> CAN_setup_errors_str = {

};

enum class CAN_teardown_errors
{
};
static const std::map<CAN_teardown_errors, std::string> CAN_teardown_errors_str = {

};

Result<std::monostate, CAN_setup_errors>    setup();
Result<std::monostate, CAN_teardown_errors> teardown();
} // namespace CanTask