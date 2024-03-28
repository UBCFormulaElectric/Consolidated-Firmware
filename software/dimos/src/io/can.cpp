#include "can.h"
#include <cstring>
#include <linux/can.h>
#include <net/if.h>
#include <optional>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

static std::optional<int> CanInterface;

Result<std::monostate, CanConnectionError> Can_Init()
{
    // Create a socket
    CanInterface = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (CanInterface < 0)
    {
        CanInterface = std::nullopt;
        return CanConnectionError::SocketError;
    }

    sockaddr_can addr;
    ifreq        ifr;

    // Get interface index
    strcpy(ifr.ifr_name, "can0");
    ioctl(CanInterface.value(), SIOCGIFINDEX, &ifr);

    // Bind the socket to the CAN interface
    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(CanInterface.value(), reinterpret_cast<const sockaddr *>(&addr), sizeof(addr)) < 0)
        return CanConnectionError::BindError;

    return std::monostate{};
}

Result<JsonCanMsg, CanReadError> Can_Read()
{
    if (!CanInterface.has_value())
        return CanReadError::ReadInterfaceNotCreated;

    can_frame     frame{};
    const ssize_t readLengthBytes =
        read(CanInterface.value(), &frame, sizeof(can_frame)); // todo make this react to QThread::requestInterruption

    if (readLengthBytes < 0)
        return CanReadError::SocketReadError;
    if (readLengthBytes < sizeof(can_frame))
        return CanReadError::IncompleteCanFrame;

    auto out = JsonCanMsg{
        .std_id = frame.can_id,
        .dlc    = frame.len8_dlc,
    };
    memcpy(out.data, frame.data, sizeof(frame.data));
    return out;
}

Result<std::monostate, CanWriteError> Can_Write(const JsonCanMsg *msg)
{
    if (!CanInterface.has_value())
        return CanWriteError::WriteInterfaceNotCreated;

    try
    {
        if (write(CanInterface.value(), msg, sizeof(can_frame)) <
            0) // todo make this react to QThread::requestInterruption
            return CanWriteError::SocketWriteError;
    }
    catch (...)
    {
        return CanWriteError::SocketWriteError;
    }
    return std::monostate{};
}