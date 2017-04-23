#pragma once
#include "WSAConnection.h"
#include "../BasicServer.hpp"

namespace meshy {
	class IocpServer : public BasicServer<WSAConnectionPtr> {
	public:
		IocpServer();

		virtual int32_t listen(std::string const& host, uint16_t port, int backlog = 20)override;
		virtual WSAConnectionPtr accept()override;

		void SetCompletionPort(HANDLE completionPort);

	private:
		HANDLE m_completionPort;
	};
}