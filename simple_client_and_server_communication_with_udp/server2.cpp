#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char ** argv)
{
	// Initializing WinSock
	WSAData wd;
	WSAStartup(MAKEWORD(2,0), &wd);

	// Setting up server socket
	SOCKET s = socket(AF_INET, SOCK_DGRAM/*UDP*/, 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(22222); //port number
	addr.sin_addr.S_un.S_addr = INADDR_ANY; // input address

	bind(s, (sockaddr*) &addr, sizeof(addr));

	// preparation for timeout control
	fd_set rd;
	timeval tv;
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	FD_ZERO(&rd);
	FD_SET(s, &rd);

	// recieving message
	char msg[1024]; // recieving buffer
	select(s + 1, &rd, NULL, NULL, &tv);
	if(FD_ISSET(s, &rd)){
		recv(s/*socket*/, msg/* data buffer */, sizeof(msg)/*1024*/, 0);
		printf("%s\n", msg);
	}else{
		printf("Comunication time out\n");
	}
	// Closing socket
	closesocket(s);

	// Destroying WinSock
	WSACleanup();

	return 0;
}
