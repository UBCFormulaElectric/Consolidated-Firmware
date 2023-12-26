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
        return SocketError;
    }

    struct sockaddr_can addr
    {
    };
    struct ifreq ifr
    {
    };

    // Get interface index
    strcpy(ifr.ifr_name, "can0");
    ioctl(CanInterface.value(), SIOCGIFINDEX, &ifr);

    // Bind the socket to the CAN interface
    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(CanInterface.value(), reinterpret_cast<const sockaddr *>(&addr), sizeof(addr)) < 0)
        return BindError;

    return std::monostate{};
}

Result<CanMsg, CanReadError> Can_Read()
{
    if (!CanInterface.has_value())
        return ReadInterfaceNotCreated;

    can_frame     frame{};
    const ssize_t readLengthBytes = read(CanInterface.value(), &frame, sizeof(can_frame));

    if (readLengthBytes < 0)
        return SocketReadError;
    if (readLengthBytes < sizeof(can_frame))
        return IncompleteCanFrame;
    //	return frame;
    return CanMsg{};
}

Result<std::monostate, CanWriteError> Can_Write(const CanMsg *msg)
{
    if (!CanInterface.has_value())
        return WriteInterfaceNotCreated;

    try
    {
        ssize_t nbytes = write(CanInterface.value(), msg, sizeof(struct can_frame));
    }
    catch (...)
    {
        return SocketWriteError;
    }
    return std::monostate{};
}