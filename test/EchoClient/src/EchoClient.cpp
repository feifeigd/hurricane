// EchoClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <thread>
#include <iostream>

int main()
{
	meshy::TcpClientPtr client = meshy::TcpClient::Connect("192.168.0.141", 9000);
	while (!client->IsConnected()) {
		client->reconnect();
		std::this_thread::sleep_for(std::chrono::seconds(3));
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

