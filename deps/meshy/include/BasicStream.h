#pragma once

#include "IStream.h"
#include "socket.h"

namespace meshy {

	class DataSink;

	class BasicStream : public IStream, public Socket {
		BasicStream(BasicStream const&) = delete;
	public:
		BasicStream(NativeSocket nativeSocket = 0);

		virtual void SetDataSink(DataSink* dataSink);
		virtual DataSink* GetDataSink();
	private:
		DataSink*	m_dataSink;
	};
}
