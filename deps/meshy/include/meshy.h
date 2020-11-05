

#pragma once

#include "IoLoop.h"
#include "ByteArray.h"
#include "ByteArrayReader.h"
#include "ByteArrayWriter.h"

#ifdef OS_LINUX
#include "epoll/EpollServer.h"
#include "epoll/EpollClient.h"
#elif defined(OS_WIN32)
#include "iocp/IocpClient.h"
#include "iocp/IocpServer.h"
#endif

#include "EventQueue.h"

// TCP的基础设施都在这，可以通过组合或者基础来扩展
namespace meshy{	
#ifdef OS_LINUX
	typedef EpollServer					TcpServer;
	typedef EpollConnection				TcpConnection;
	typedef EpollClient					TcpClient;
	typedef EpollStream					TcpStream;
#elif defined(OS_WIN32)
	typedef IocpServer					TcpServer;
	typedef WSAConnection				TcpConnection;
	typedef IocpClient					TcpClient;
	typedef IocpStream					TcpStream;
#endif	
	typedef std::shared_ptr<TcpClient>	TcpClientPtr;
}
