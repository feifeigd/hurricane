// EchoClient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <thread>
#include <iostream>

int main()
{
	TRACE_DEBUG("hello {0}", "world");
	size_t time = 3;
	meshy::TcpClientPtr client = meshy::TcpClient::Connect("192.168.31.206", 9000);
	while (!client->IsConnected()) {
		client->reconnect();
		std::this_thread::sleep_for(std::chrono::seconds(++time));
	}
	while (true) {
		std::string str;
		std::cin >> str;
		if ("exit" == str)
		{
			TRACE_DEBUG("exit");
			break;
		}
		client->send(str);
	}
    return 0;
}

