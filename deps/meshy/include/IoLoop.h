#pragma once

#include <cstdint>
#if OS_LINUX
using std::size_t;
#endif // OS_LINUX

size_t const MAX_EVENT_COUNT = 5000;
size_t const MAX_RECV_BUFF = 65535;

#ifdef OS_WIN32
#include "iocp/IocpLoop.h"
typedef meshy::IocpLoop IoLoop;
#elif defined(OS_LINUX)
#include "epoll/EpollLoop.h"
typedef meshy::EpollLoop IoLoop;
#endif // OS_WIN32
