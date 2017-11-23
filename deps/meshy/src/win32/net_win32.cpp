
#include <win32/net_win32.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
using meshy::WindowsSocketInitializer;

WindowsSocketInitializer::WindowsSocketInitializer() {
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	DWORD err = WSAStartup(wVersionRequested, &wsaData);
	if (err)
	{
		std::cerr << "Request Windows Socket Library Error!" << std::endl;
		throw std::exception("Request Windows Socket Library Error!");
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		WSACleanup();
		std::cerr << "Request Windows Socket Version 2.2 Error!" << std::endl;
		throw std::exception("Request Windows Socket Version 2.2 Error!");
	}
}

WindowsSocketInitializer::~WindowsSocketInitializer() {
	WSACleanup();
}

void WindowsSocketInitializer::initialize() {
	static WindowsSocketInitializer init;
}
