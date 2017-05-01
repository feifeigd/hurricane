
#pragma once

#include "EpollStream.h"
#include "../IConnectable.h"

#include <memory>

namespace meshy{
	class EpollClient;
	typedef std::shared_ptr<EpollClient> EpollClientPtr;

	class EpollClient : public EpollStream, public IConnectable{
		EpollClient(EpollClient const&) = delete;
	public:
		virtual void connect(std::string const& host, uint16_t port)override;
		static EpollClientPtr Connect(std::string const& host, uint16_t port, DataSink* dataSink);

	private:
		EpollClient(NativeSocket socket);
	};
}

