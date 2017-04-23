#pragma once

#include "../loop.h"
#include "../utils/concurrent_queue.h"
#include "Iocp.h"
#include "WSAConnection.h"
#include "../win32/net_win32.h"
#include <string>
#include <map>

namespace meshy {

	class IocpServer;

	class IocpLoop : public Loop {
	public:
		static IocpLoop& get();
		void AddServer(IocpServer* server);

	private:
		IocpLoop();

	protected:
		virtual void run()override;

	private:
		void IocpThread();
		void IocpConnectionThread(IocpServer* server);
		void WorkThread(NativeSocket listenfd, HANDLE completionPort);
		void enqueue(IocpStreamPtr stream, char const* buf, size_t nread);
	private:
		bool										m_shutdown;
		SYSTEM_INFO									m_systemInfo;

		ConcurrentQueue<IocpServer*>				m_serverQueue;
		std::map<NativeSocket, IocpServer*>			m_servers;
		std::map<NativeSocket, WSAConnectionPtr>	m_streams;
	};
}
