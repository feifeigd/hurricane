#pragma once

#include "../loop.h"
#include "../utils/concurrent_queue.h"
#include "Iocp.h"
//#include "WSAConnection.h"
#include "../win32/net_win32.h"
#include <string>
#include <map>

#include <thread>

namespace meshy {

	class IocpServer;

	class IocpLoop : public Loop {
	public:
		static IocpLoop& get();
		void AddServer(IocpServer* server);

		static LPFN_ACCEPTEX				lpAcceptEx;	///< 函数指针
		static LPFN_GETACCEPTEXSOCKADDRS	lpGetAcceptExSockaddrs;

		/// 收到了数据
		void enqueue(IocpStream* stream, char const* buf, size_t nread);
	private:
		IocpLoop();
		~IocpLoop();

		void stop();

	protected:
		virtual void run()override;

	private:
		bool init();
		void IocpThread();
		
		void WorkThread();
	private:
		bool										m_shutdown = true;
		SYSTEM_INFO									m_systemInfo;

		ConcurrentQueue<IocpServer*>				m_serverQueue;

		mutable std::mutex							m_serverMutex;
		std::map<NativeSocket, IocpServer*>			m_servers;
		std::vector<std::thread>					m_thread_group;
		HANDLE										m_completionPort = Iocp::CreateCompletionPort();
	};
}
