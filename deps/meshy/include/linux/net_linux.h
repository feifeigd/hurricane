#pragma once

#include <sys/epoll.h>
#include <cstdint>

namespace meshy{
	typedef int32_t NativeSocket;
	typedef ::epoll_event NativeSocketEvent;
}
