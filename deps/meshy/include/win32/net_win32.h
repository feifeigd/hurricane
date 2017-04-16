#pragma once

#include <WinSock2.h>

namespace meshy {

	typedef SOCKET NativeSocket;
	typedef SOCKADDR_IN NativeSocketAddress;

	class WindowsSocketInitializer {
		WindowsSocketInitializer();
	public:
		static void initialize();
	};
}