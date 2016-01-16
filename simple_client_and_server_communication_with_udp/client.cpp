#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char ** argv){
	// Initializing WinSock
	WSAData wd;
	WSAStartup(MAKEWORD(2,0), &wd);

	// Setting up client socket
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(22222);
	addr.sin_addr.S_un.S_addr = inet_addr("172.18.1.10");

	// Sending message
	char msg[1024] = "Hello world";

	sendto(s, msg, strlen(msg)+1, 0, (sockaddr*) &addr, sizeof(addr));

	// Closing socket
	closesocket(s);

	// Destroying WinSock
	WSACleanup();
	return 0;
}
