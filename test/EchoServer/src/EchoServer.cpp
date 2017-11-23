// TestTcpServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <EchoSink.h>

int main()
{
	EchoSink sink;
	meshy::TcpServer server(&sink);
	server.listen("0.0.0.0", 9000);
	bool bRun = true;
	while (bRun)
		;
    return 0;
}

