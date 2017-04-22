
#pragma once

#include "EpollConnection.h"

namespace meshy{
	class EpollServer{
	public:
		EpollConnectionPtr accept(NativeSocket sockfd);

	};	
}

