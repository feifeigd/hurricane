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

	protected:
		virtual void run()override;

	private:
		void EpollThread();
		void HandleEvent(NativeSocketEvent* events, int32_t nfds);
		void accept(int32_t listenfd);
		void read(int32_t fd, uint32_t events);

	private:
		int32_t									m_eventfd;
		bool									m_shutdown;
		std::map<NativeSocket, EpollServer*>	m_servers;
		std::map<NativeSocket, EpollStreamPtr>	m_streams;
	};
}
