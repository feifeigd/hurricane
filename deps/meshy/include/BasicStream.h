#pragma once

#include "IStream.h"
#include "socket.h"

namespace meshy {

	class DataSink;

	class BasicStream : public IStream, public Socket {
		BasicStream(BasicStream const&) = delete;
	public:
		BasicStream(NativeSocket nativeSocket = -1, NativeSocketAddress const& address = { 0 });

		void SetDataSink(DataSink* dataSink);
		DataSink* GetDataSink();

		virtual void OnDataIndication(DataIndicationHandler handler)override { m_dataHandler = handler; }
		virtual void disconnect()override;
		void SetConnected(bool connected){m_bIsConnected = connected;}
		bool IsConnected()const{return m_bIsConnected;};
	private:
		bool					m_bIsConnected = false;
		DataIndicationHandler	m_dataHandler;
		DataSink*				m_dataSink = nullptr;
	};
}
