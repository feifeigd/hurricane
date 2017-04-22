
#pragma once

#include "EpollStream.h"
#include <memory>

namespace meshy{
	
	class EpollConnection : public EpollStream{
	};
	typedef std::shared_ptr<EpollConnection>	EpollConnectionPtr;
}

