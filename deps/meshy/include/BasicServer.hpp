#pragma once

#include "socket.h"
#include "utils/common_utils.h"
#include "utils/logger.h"
#include <cassert>
#include <functional>

namespace meshy {
	
	class IStream;

	template<typename ConnectionType>
	class BasicServer : public Socket {
	public:
		typedef std::function<void(IStream*)> ConnectIndicationHandler;
		typedef std::function<void(IStream*)> DisconnectIndicationHandler;

	protected:
		int32_t bind(std::string const& host, uint16_t port) {
			NativeSocket listenfd = socket(AF_INET, SOCK_STREAM, 0);
			if (listenfd < 0)
			{
				TRACE_ERROR("Create socket failed!");
				exit(1);
			}
			int32_t option = 1;
			setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&option, sizeof(option));
			SetNonBlocking(listenfd);
			NativeSocketAddress srvAddr = {0};
#ifdef OS_WIN32
			inet_pton(AF_INET, host.c_str(), &srvAddr.sin_addr);
#elif defined(OS_LINUX)
			inet_aton(host.c_str(), &srvAddr.sin_addr);
#endif // OS_WIN32

			//srvAddr.sin_addr.s_addr = inet_addr(host.c_str());
			srvAddr.sin_family = AF_INET;
			srvAddr.sin_port = htons(port);
			int32_t errorCode = ::bind(listenfd, (sockaddr*)&srvAddr, sizeof(sockaddr));
			if (errorCode < 0)
			{
				TRACE_ERROR("Bind failed. Error: " + errno);
				assert(false);
				return errorCode;
			}
			SetNativeSocket(listenfd);
			return 0;
		}

	public:
		void OnConnectIndication(ConnectIndicationHandler handler){
			m_connectHandler = handler;
		}

		void OnDisconnectIndication(DisconnectIndicationHandler handler){
			m_disconnectIndication = handler;
		}
		virtual int32_t listen(std::string const& host, uint16_t port, int backlog) = 0;
		virtual ConnectionType accept() = 0;

	protected:
		ConnectIndicationHandler	m_connectHandler;
		DisconnectIndicationHandler	m_disconnectIndication;
	};
}
