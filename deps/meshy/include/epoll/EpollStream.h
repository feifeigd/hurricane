
#pragma once

#include <BasicStream.h>
#include <memory>

namespace meshy{
	class EpollStream : public BasicStream{
		EpollStream(EpollStream const&) = delete;
	public:
		EpollStream(NativeSocket socket, NativeSocketAddress const& address = { 0 });

		virtual size_t receive(char* buffer, size_t bufferSize, size_t& readSize)override;
		virtual size_t send(ByteArray const& byteArray)override;

		void events(uint32_t evts);
		uint32_t events()const;
	protected:
		uint32_t	m_events = 0;
	};

	typedef std::shared_ptr<EpollStream>	EpollStreamPtr;
}


