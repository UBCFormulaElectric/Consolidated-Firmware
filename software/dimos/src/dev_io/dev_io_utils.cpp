#include "dev_io_utils.h"
#include <thread>
void wait_delegate_thread()
{
    for (int i = 0; i < 100000000; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(0));
    }
}