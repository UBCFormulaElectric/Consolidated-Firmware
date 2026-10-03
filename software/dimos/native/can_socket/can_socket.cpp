#include "can_socket.h"

#include <cerrno>

#ifdef __linux__

#include <cstring>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

static int32_t closeWithError(const int fd, const int error)
{
    close(fd);
    return -error;
}

int32_t dimos_can_open(const char* ifname)
{
    if (ifname == nullptr || std::strlen(ifname) >= IFNAMSIZ)
    {
        return -EINVAL;
    }

    const int fd = socket(PF_CAN, SOCK_RAW | SOCK_CLOEXEC, CAN_RAW);
    if (fd < 0)
    {
        return -errno;
    }

    ifreq ifr{};
    std::strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
    if (ioctl(fd, SIOCGIFINDEX, &ifr) < 0)
    {
        return closeWithError(fd, errno);
    }
    // ifr_ifindex and ifr_flags share a union, so read the index before querying flags.
    const int ifindex = ifr.ifr_ifindex;
    if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0)
    {
        return closeWithError(fd, errno);
    }
    if ((ifr.ifr_flags & IFF_UP) == 0)
    {
        return closeWithError(fd, ENETDOWN);
    }

    // Without this the kernel only delivers classic (<= 8 byte) frames to the socket.
    const int enable_fd_frames = 1;
    if (setsockopt(fd, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &enable_fd_frames, sizeof(enable_fd_frames)) < 0)
    {
        return closeWithError(fd, errno);
    }

    sockaddr_can addr{};
    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifindex;
    if (bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
    {
        return closeWithError(fd, errno);
    }

    return fd;
}

int32_t dimos_can_read(int32_t fd, int32_t timeout_ms, uint32_t* can_id, uint8_t* len, uint8_t* data)
{
    pollfd pfd{ fd, POLLIN, 0 };
    const int ready = poll(&pfd, 1, timeout_ms);
    if (ready == 0 || (ready < 0 && errno == EINTR))
    {
        return 0;
    }
    if (ready < 0)
    {
        return -errno;
    }

    // A classic can_frame is a prefix of canfd_frame, so one buffer reads both.
    canfd_frame frame{};
    const ssize_t bytes = read(fd, &frame, sizeof(frame));
    if (bytes < 0)
    {
        return errno == EINTR || errno == EAGAIN ? 0 : -errno;
    }
    if (bytes != CAN_MTU && bytes != CANFD_MTU)
    {
        return -EIO;
    }
    if ((frame.can_id & (CAN_ERR_FLAG | CAN_RTR_FLAG)) != 0)
    {
        return 0;
    }

    *can_id = (frame.can_id & CAN_EFF_FLAG) != 0 ? frame.can_id & CAN_EFF_MASK : frame.can_id & CAN_SFF_MASK;
    *len    = frame.len > DIMOS_CAN_MAX_PAYLOAD ? DIMOS_CAN_MAX_PAYLOAD : frame.len;
    std::memcpy(data, frame.data, *len);
    return 1;
}

void dimos_can_close(int32_t fd)
{
    if (fd >= 0)
    {
        close(fd);
    }
}

#else

// SocketCAN only exists on Linux; other platforms use the UDP source.
int32_t dimos_can_open(const char*)
{
    return -ENOTSUP;
}

int32_t dimos_can_read(int32_t, int32_t, uint32_t*, uint8_t*, uint8_t*)
{
    return -ENOTSUP;
}

void dimos_can_close(int32_t) {}

#endif
