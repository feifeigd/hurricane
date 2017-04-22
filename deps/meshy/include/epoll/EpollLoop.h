#pragma once

#include "../loop.h"
#include "../linux/common.h"
#include "../linux/net_linux.h"
#include "EpollStream.h"

#include <map>

namespace meshy {

	class EpollServer;

	class EpollLoop : public Loop {
	public:
		static EpollLoop& get();
		virtual ~EpollLoop();
		
		void AddServer(NativeSocket socket, EpollServer* server);
		void AddStream(EpollStreamPtr stream);

		/// 对epoll_ctl的简单封装
		int32_t AddEpollEvents(int32_t events, NativeSocket fd);
		int32_t ModifyEpollEvents(int32_t events, NativeSocket fd);

	protected:
		EpollLoop();
		virtual void run()override;

	private:
		void initialize();
		void EpollThread();
		void HandleEvent(NativeSocketEvent* events, int32_t nfds);
		void accept(NativeSocket listenfd);
		void read(NativeSocket fd, uint32_t events);
		void enqueue(EpollStreamPtr connection, char const* buf, size_t nread);

	private:
		int32_t									m_eventfd;
		bool									m_shutdown;
		std::map<NativeSocket, EpollServer*>	m_servers;
		std::map<NativeSocket, EpollStreamPtr>	m_streams;
	};
}
