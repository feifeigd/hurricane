#include <BasicStream.h>

using meshy::BasicStream;
using meshy::NativeSocket;
using meshy::Socket;
using meshy::DataSink;

BasicStream::BasicStream(NativeSocket nativeSocket)
	: Socket(nativeSocket), m_dataSink(nullptr)
{

}

void BasicStream::SetDataSink(DataSink* dataSink) {
	m_dataSink = dataSink;
}

DataSink* BasicStream::GetDataSink() {
	return m_dataSink;
}
