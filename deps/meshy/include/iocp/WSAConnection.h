#pragma once

#include "IocpStream.h"

namespace meshy {
	class WSAConnection : public IocpStream {
		WSAConnection(WSAConnection const&) = delete;
	public:
		WSAConnection(NativeSocket socket, NativeSocketAddress const& address);
	};
	typedef std::shared_ptr<WSAConnection> WSAConnectionPtr;
}