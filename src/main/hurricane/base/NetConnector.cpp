#include <hurricane/base/NetConnector.h>

#include <thread>

using meshy::ByteArray;
using meshy::TcpClient;

void NetConnector::connect() {
	m_client = meshy::TcpClient::Connect(m_host.host(), m_host.port());
}

size_t NetConnector::SendAndReceive(char const* buffer, size_t size, char* resultBuffer, size_t resultSize) {
	m_client->send(ByteArray(buffer, size));
	bool receivedData = false;
	m_client->OnDataIndication([&](char const* buf, size_t size) {
		if (resultSize > size)resultSize = size;
		memcpy(resultBuffer, buf, resultSize);
		receivedData = true;
	});
	while (!receivedData)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	return resultSize;
}
