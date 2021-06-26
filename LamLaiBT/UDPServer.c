#include <WinSock2.h>
#include <WS2tcpip.h>
#include <ws2def.h>
#include <stdio.h>
#define BUFF_SIZE 2048;
#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"


#pragma comment(lib,"Ws2_32.lib")

//
int main(int argc, char* argv[]) {
	
	//Buoc1: Khoi tao Winsock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion,&wsaData)) {
		printf("Winsock 2.2 is not supported\n");
		return 0;
	}

	//Buoc2: Tao socket
	SOCKET server;
	server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (server == INVALID_SOCKET) {
		printf("ERROR %d: Khong tao duoc server", WSAGetLastError());
		return 0;
	}

	//Buoc3: Gan Socket - o cam vao dia chi
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);
	if (bind(server, (SOCKADDR*)&serverAddr, sizeof(serverAddr)))
	{
		printf("ERROR %d: Khong gan duoc dia chi cho socket", WSAGetLastError());
		return 0;
	}

	//Buoc4: Giao tiep voi client
	SOCKADDR_IN clientAddr;
	char buff[2048], clientIP[INET_ADDRSTRLEN];
	int ret, clientAddrLen = sizeof(clientAddr), clientPort;

	while (1) {
		ret = recvfrom(server, buff, 2048, 0, (SOCKADDR*)&clientAddr, &clientAddrLen);
		if (ret == SOCKET_ERROR)
			printf("ERROR %d: Khong nhan duoc du lieu", WSAGetLastError());
		else if (strlen(buff) > 0) {
			buff[ret] = 0;
			inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
			clientPort = ntohs(clientAddr.sin_port);
			printf("Receive from client[%s:%d] %s\n", clientIP, clientPort, buff);

			//Gui lai Client
			ret = sendto(server, buff, strlen(buff), 0, (SOCKADDR *)&clientAddr, sizeof(clientAddr));
			if(ret == SOCKET_ERROR)
				printf("ERROR %d: Khong gui duoc du lieu", WSAGetLastError());
		}
	}

	closesocket(server);
	WSACleanup();

	printf("OK");
	return 0;
}