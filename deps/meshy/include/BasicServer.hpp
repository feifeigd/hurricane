#pragma once

#include "socket.h"
#include <functional>

namespace meshy {
	template<typename ConnectionType>
	class BasicServer : public Socket {
	public:
		typedef std::function<void(IStream*)> ConnectIndicationHandler;
		typedef std::function<void(IStream*)> DisconnectIndicationHandler;

		virtual int32_t listen(std::string const& host, uint16_t port, int backlog) = 0;
		virtual void OnConnectIndication(ConnectIndicationHandler handler) = 0;
		virtual void OnDisconnectIndication(DisconnectIndicationHandler handler) = 0;

		virtual ConnectionType accept(int32_t listenfd) = 0;
	};
}
