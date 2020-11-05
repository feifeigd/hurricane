
#include <WinSock2.h>	// WSADATA
#include <WS2tcpip.h>	// IP_MULTICAST_TTL
#include <iostream>
#include <functional>
#include <thread>
using namespace std;

#define TTL 64
#define BUF_SIZE 600

void error_handling(string const& msg) {
	cerr << msg << endl;
	exit(1);
}

void SendErrorMsg(SOCKET clntSock) {
	char msg[] = "HTTP/1.0 400 Bad Request\r\n"
		"Server: simple web server\r\n"
		"Content-length: 2048\r\n"
		"Content-type: text/html\r\n\r\n"
		"<html><head><title>Network</title></head><body>发生错误，查看请求文件名和请求方式！</body></html>";
	send(clntSock, msg, sizeof(msg), 0);
}

char const* ContentType(char const* file) {
	char fileName[BUF_SIZE];
	strcpy(fileName, file);
	strtok(fileName, ".");
	string extension = strtok(nullptr, ".");
	if (extension == "html" || extension == "htm")
	{
		return "text/html";
	}
	return "text/plain";
}

void SendData(SOCKET sock, char* ct, char* fileName) {
	FILE* sendFile = nullptr;
	if (!(sendFile = fopen(fileName, "r")))
	{
		SendErrorMsg(sock);
		return;
	}
	char header[] = "HTTP/1.0 200 OK\r\n"
		"Server: simple web server\r\n"
		"Content-length: 2048\r\n"
		"Content-type: %s\r\n\r\n";
	char buf[BUF_SIZE];
	int len = sprintf(buf, header, ct);
	send(sock, buf, len + 1, 0);
	while (fgets(buf, BUF_SIZE, sendFile))
	{
		send(sock, buf, strlen(buf), 0);
	}
	closesocket(sock);
}

bool RequestHandler(SOCKET clntSock) {
	char buf[BUF_SIZE];
	recv(clntSock, buf, BUF_SIZE, 0);
	char* p = buf;
	if ( !strstr(buf, "HTTP/"))
	{
		SendErrorMsg(clntSock);
		closesocket(clntSock);
		return false;
	}
	char method[BUF_SIZE];
	strcpy(method, strtok(p, " /"));
	if (string("GET") != method)
	{
		SendErrorMsg(clntSock);
	}
	p += strlen(method) + 2;
	char fileName[BUF_SIZE];
	strcpy(fileName, strtok(p, " /"));
	char ct[BUF_SIZE];
	strcpy(ct, ContentType(fileName));
	SendData(clntSock,  ct, fileName);
	return true;
}

// http://localhost:9999/news.html
int main() {
	WSADATA wsaData{};
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		error_handling("WSAStartup() error");
	}
	SOCKET hServSock = socket(PF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN servAdr{}, clntAdr{};
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(9999);
	if (SOCKET_ERROR == ::bind(hServSock, (SOCKADDR*)& servAdr, sizeof(servAdr)))
	{
		error_handling("bind() error.");
	}
	if (SOCKET_ERROR == listen(hServSock, 5))
	{
		error_handling("listen() error.");
	}
	int clntAdrSize = sizeof(clntAdr);
	while (true)
	{
		SOCKET clntSock = accept(hServSock, (SOCKADDR*)& clntAdr, &clntAdrSize);
		printf("Connection Request: %s:%d\n", inet_ntoa(clntAdr.sin_addr), ntohs(clntAdr.sin_port));
		thread t(std::bind(RequestHandler, clntSock));
		t.detach();
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}
