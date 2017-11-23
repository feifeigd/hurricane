
#pragma once

#include "EpollStream.h"
#include <memory>

namespace meshy{
	
	class EpollConnection : public EpollStream{
		EpollConnection(EpollConnection const&) = delete;
	public:
		EpollConnection(NativeSocket socket, NativeSocketAddress const& address);

	};
	typedef std::shared_ptr<EpollConnection>	EpollConnectionPtr;
}

