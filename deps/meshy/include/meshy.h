

#pragma once

#include "IoLoop.h"
#include "ByteArray.h"

#ifdef OS_LINUX
#include "epoll/EpollServer.h"
#elif defined(OS_WIN32)
#include "iocp/IocpServer.h"
#endif

#include "EventQueue.h"

namespace meshy{
#ifdef OS_LINUX
	typedef EpollServer		TcpServer;
	typedef EpollConnection	TcpConnection;
#elif defined(OS_WIN32)
	typedef IocpServer		TcpServer;
	typedef WSAConnection	TcpConnection;
#endif	
}

