
#pragma once

#include "NetAddress.h"
#include <meshy.h>

typedef std::function<void(meshy::IStream* connection, char const* buffer, size_t size)>	DataReceiver;

class NetListener
{
public:
	NetListener(hurricane::base::NetAddress const& host):m_host(host){}
	
	void listen();
	void OnData(DataReceiver receiver) { m_receiver = receiver; }
private:
	hurricane::base::NetAddress	m_host;
	DataReceiver				m_receiver;
	meshy::TcpServer			m_server;
};
