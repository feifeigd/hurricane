#pragma once

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <cstdint>
#include <dlfcn.h>

namespace meshy{
	typedef int32_t NativeSocket;
	typedef ::sockaddr_in	NativeSocketAddress;
	typedef ::epoll_event NativeSocketEvent;
}
