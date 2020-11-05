
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
	SOCKET hSendSock = socket(PF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN mulAdr{};
	mulAdr.sin_family = AF_INET;
	mulAdr.sin_addr.s_addr = inet_addr("224.1.1.2");
	mulAdr.sin_port = htons(9999);
	//if (SOCKET_ERROR == bind(hSendSock, (SOCKADDR*)& mulAdr, sizeof(mulAdr)))
	//	error_handling("bind() error");

	int timeLive = TTL;
	setsockopt(hSendSock, IPPROTO_IP, IP_MULTICAST_TTL, (char const*)&timeLive, sizeof(timeLive));
	FILE* fp = nullptr;
	if (!(fp = fopen("news.txt", "r"))) {
		error_handling("fopen() error");
	}
	char buf[BUF_SIZE];
	while (!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(hSendSock, buf, strlen(buf), 0, (SOCKADDR*)& mulAdr, sizeof(mulAdr));
		//sendto(hSendSock, buf, strlen(buf), 0, nullptr, 0);
		Sleep(2000);
	}
	closesocket(hSendSock);
	fclose(fp);
	WSACleanup();
	return 0;
}
