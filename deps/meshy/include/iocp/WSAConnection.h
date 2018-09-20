#pragma once

#include "IocpStream.h"
#include "BasicServer.hpp"

namespace meshy {

	class WSAConnection;
	class IocpServer;

	typedef std::shared_ptr<WSAConnection> WSAConnectionPtr;
	//typedef BasicServer<WSAConnectionPtr> WSABasicServer;

	class WSAConnection : public IocpStream {
		WSAConnection(WSAConnection const&) = delete;
	public:
		WSAConnection(IocpServer* server, NativeSocket socket);
		~WSAConnection();
		virtual void disconnect()override;

		IocpServer* server()const;
	private:
		IocpServer * m_server;
	};
}