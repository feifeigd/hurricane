

#pragma once

#include "IoLoop.h"

#ifdef OS_LINUX
#include "epoll/EpollServer.h"
#elif defined(OS_WIN32)
#include "iocp/IocpServer.h"
#endif

#include "EventQueue.h"

namespace meshy{
#ifdef OS_LINUX
	typedef EpollServer	TcpServer;
#elif defined(OS_WIN32)
	typedef IocpServer	TcpServer;
#endif	
}

