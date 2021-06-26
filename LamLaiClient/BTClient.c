#include <WinSock2.h>
#include <WS2tcpip.h>
#include <ws2def.h>
#include <stdio.h>
#include <string.h>
#define BUFF_SIZE 2048;
#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"

#pragma comment(lib,"Ws2_32.lib")

int main(int argc, char* argv[]) {
	//Tao winsock va socket thi tuong tu nhu server
	//Buoc1: Khoi tao Winsock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData)) {
		printf("Winsock 2.2 is not supported\n");
		return 0;
	}

	//Buoc2: Tao socket
	SOCKET client;
	client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (client == INVALID_SOCKET) {
		printf("ERROR %d: Khong tao duoc server", WSAGetLastError());
		return 0;
	}

	printf("Client da chay");

	//(optional) dat thoi gian time out
	int tv = 10000;
	setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)(&tv), sizeof(int));

	//Buoc 3: ghi ra thong tin server
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);

	//Buoc 4: giao tiep voi server
	char buff[2048];
	int ret, serverAddrLen = sizeof(serverAddr), messageLen;
	while (1) {
		printf("Gui toi server: ");
		gets_s(buff, 2048);
		messageLen = strlen(buff);
		if (messageLen == 0) break;

		ret = sendto(client, buff, messageLen, 0, (SOCKADDR*)&serverAddr, serverAddrLen);
		if (ret == SOCKET_ERROR)
			printf("Error %d: khong the gui duoc message", WSAGetLastError());

		//Nhan message phan hoi
		ret = recvfrom(client, buff, 2048, 0, (SOCKADDR*)&serverAddr, &serverAddrLen);
		if (ret == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAETIMEDOUT)
				printf("Het gio!");
			else printf("Error %d: khong the nhan duoc message", WSAGetLastError());
		}
		else if (strlen(buff) > 0) {
			buff[ret] = 0;
			printf("Nhan tu server: %s\n", buff);
		}
	}

	closesocket(client);
	WSACleanup();
	return 0;
}
