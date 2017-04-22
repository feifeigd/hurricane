#pragma once

#include <cstdint>
using std::size_t;

size_t const MAX_EVENT_COUNT = 5000;
size_t const MAX_RECV_BUFF = 65535;

#ifdef OS_WIN32
#include "iocp/iocploop.h"
#define IoLoop IocpLoop
#elif defined(OS_LINUX)
#include "epoll/EpollLoop.h"
#define IoLoop EpollLoop
#endif // OS_WIN32
