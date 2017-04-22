
#pragma once

#include <BasicStream.h>
#include <memory>

namespace meshy{
	class EpollStream : public BasicStream{
	public:
	};

	typedef std::shared_ptr<EpollStream>	EpollStreamPtr;
}


