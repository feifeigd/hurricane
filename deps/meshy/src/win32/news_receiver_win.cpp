
#include <WinSock2.h>	// WSADATA
#include <WS2tcpip.h>	// IP_MULTICAST_TTL
#include <iostream>
using namespace std;

#define TTL 64
#define BUF_SIZE 6

void error_handling(string const& msg) {
	cerr << msg << endl;
	exit(1);
}

int main() {
	WSADATA wsaData{};
	if (WSAStartup(MAKEWORD(2,2), &wsaData))
	{
		error_handling("WSAStartup() error");
	}
	SOCKET hRecvSock = socket(PF_INET, SOCK_DGRAM, 0);
	// 绑定端口
	SOCKADDR_IN adr{};
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = htonl(INADDR_ANY);
	adr.sin_port = htons(9999);
	if(SOCKET_ERROR == bind(hRecvSock, (SOCKADDR*)&adr, sizeof(adr)))
		error_handling("bind() error");
	ip_mreq joinAdr{};
	joinAdr.imr_multiaddr.s_addr = inet_addr("224.1.1.2");
	joinAdr.imr_interface.s_addr = htonl(INADDR_ANY);
	
	if(SOCKET_ERROR == setsockopt(hRecvSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char const*)&joinAdr, sizeof(joinAdr)))
		error_handling("setsockopt() error");
	
	char buf[BUF_SIZE];
	while (true)
	{
		int len = recvfrom(hRecvSock, buf, BUF_SIZE - 1, 0, nullptr, 0);
		if(len < 0)break;
		buf[len] = 0;
		fputs(buf, stdout);
	}
	closesocket(hRecvSock);
	WSACleanup();
	return 0;
}
