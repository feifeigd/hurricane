#pragma once
#include "WSAConnection.h"
#include "../BasicServer.hpp"

namespace meshy {
	class IocpServer : public BasicServer<WSAConnectionPtr> {
	public:
		IocpServer(DataSink* dataSink = nullptr);

		virtual int32_t listen(std::string const& host, uint16_t port, int backlog = SOMAXCONN)override;
		virtual ConnectionType accept(NativeSocket fd)override;

		void SetCompletionPort(HANDLE completionPort);

		void PostAccept();

	private:
		HANDLE m_completionPort;
	};
}