#pragma once

#include "IocpStream.h"
#include "BasicServer.hpp"

namespace meshy {

	class WSAConnection;
	typedef std::shared_ptr<WSAConnection> WSAConnectionPtr;
	typedef BasicServer<WSAConnectionPtr> WSABasicServer;

	class WSAConnection : public IocpStream {
		WSAConnection(WSAConnection const&) = delete;
	public:
		WSAConnection(WSABasicServer* server, NativeSocket socket);
		~WSAConnection();
		virtual void disconnect()override;
	private:
		WSABasicServer* m_server;
	};
}