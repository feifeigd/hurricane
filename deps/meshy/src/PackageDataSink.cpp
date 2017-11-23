#include <http/HttpRequest.h>
#include <http/HttpResponse.h>
#include <IStream.h>
#include <PackageDataSink.h>
#include <utils/logger.h>

using meshy::ByteArray;
using meshy::EventQueue;
using meshy::HttpRequest;
using meshy::HttpResponse;
using meshy::IStream;
using meshy::PackageDataSink;

PackageDataSink::PackageDataSink(EventQueue* eventQueue)
	: m_eventQueue(eventQueue), m_totalSize(0)
	, m_threadPool(10, [](BaseEvent& event) {
		TRACE_DEBUG("Thread onEvent sink!");
		std::string requestText = event.data().ToStdString();
		HttpRequest request = HttpRequest::FromStdString(requestText);
		HttpResponse response;
		response.version("HTTP/1.1");
		response.SetStatusCode(200);
		response.content("Hello!Sink in thread!");
		event.stream()->send(response.ToStdString());
		TRACE_DEBUG("Thread onEvent sink end.");
		event.stream()->disconnect();
	})
{

}

PackageDataSink::~PackageDataSink() {

}

size_t PackageDataSink::OnDataIndication(IStream* stream, char const* buf, size_t bytes) {
	m_data.concat(ByteArray(buf, bytes));
	if (m_data.size() >= m_totalSize)
	{
		m_threadPool.submit(BaseEvent("data", m_data, stream));
		m_data.clear();
		m_totalSize = 0;
	}
	return bytes;
}
