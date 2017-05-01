#include <hurricane/base/NetListener.h>

void NetListener::listen() {
	IoLoop::get().start();

	m_server.listen(m_host.host(), m_host.port());

	m_server.OnConnectIndication([&](meshy::IStream* stream) {
		stream->OnDataIndication([&](char const* buffer, size_t size) {
			m_receiver(stream, buffer, size);
		});
	});
}