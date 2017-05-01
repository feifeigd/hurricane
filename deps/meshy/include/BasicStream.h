#pragma once

#include "IStream.h"
#include "socket.h"

namespace meshy {

	class DataSink;

	class BasicStream : public IStream, public Socket {
		BasicStream(BasicStream const&) = delete;
	public:
		BasicStream(NativeSocket nativeSocket = 0);

		void SetDataSink(DataSink* dataSink);
		DataSink* GetDataSink();

		virtual void OnDataIndication(DataIndicationHandler handler)override { m_dataHandler = handler; }
	private:
		DataIndicationHandler	m_dataHandler;
		DataSink*	m_dataSink;
	};
}
