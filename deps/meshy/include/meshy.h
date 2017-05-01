

#pragma once

#include "IoLoop.h"
#include "ByteArray.h"

#ifdef OS_LINUX
#include "epoll/EpollServer.h"
#include "epoll/EpollClient.h"
#elif defined(OS_WIN32)
#include "iocp/IocpServer.h"
#endif

#include "EventQueue.h"

namespace meshy{
#ifdef OS_LINUX
	typedef EpollServer		TcpServer;
	typedef EpollConnection	TcpConnection;
	typedef EpollClient		TcpClient;
	typedef EpollStream		TcpStream;
#elif defined(OS_WIN32)
	typedef IocpServer		TcpServer;
	typedef WSAConnection	TcpConnection;
	//typedef IocpClient		TcpClient;
	typedef IocpStream		TcpStream;
#endif	
}

