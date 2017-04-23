
#pragma once

#include "../BasicServer.hpp"
#include "EpollConnection.h"

namespace meshy{
	class EpollServer : public BasicServer<EpollConnectionPtr> {
	public:
		virtual int32_t bind(std::string const& host, uint16_t port);
		EpollConnectionPtr accept(NativeSocket sockfd);

	};	
}

