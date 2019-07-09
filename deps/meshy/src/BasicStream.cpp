#include <BasicStream.h>

using meshy::BasicStream;
using meshy::NativeSocket;
using meshy::NativeSocketAddress;
using meshy::Socket;
using meshy::DataSink;

BasicStream::BasicStream(NativeSocket nativeSocket, NativeSocketAddress const& address)
	: Socket(nativeSocket, address)
{

}

void BasicStream::SetDataSink(DataSink* dataSink) {
	m_dataSink = dataSink;
}

DataSink* BasicStream::GetDataSink() {
	return m_dataSink;
}

void BasicStream::disconnect()
{
	m_bIsConnected = false;
}
