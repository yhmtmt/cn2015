#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char ** argv)
{
	// Initializing WinSock
	WSAData wd;
	WSAStartup(MAKEWORD(2,0), &wd);

	// Setting up server socket
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(22222);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(s, (sockaddr*) &addr, sizeof(addr));

	// recieving message
	char msg[1024];
	recv(s, msg, sizeof(msg), 0);
	printf("%s\n", msg);
	
	// Closing socket
	closesocket(s);

	// Destroying WinSock
	WSACleanup();

	return 0;
}
