// TestTcpServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <EchoSink.h>
#include <thread>
#include <chrono>

int main()
{
	EchoSink sink;
	meshy::TcpServer server(&sink);
	server.listen("0.0.0.0", 9000);
	bool bRun = true;
	while (bRun)
		std::this_thread::sleep_for(std::chrono::microseconds(200));
    return 0;
}

