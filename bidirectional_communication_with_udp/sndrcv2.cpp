
#include <iostream>
#include <thread>
#include <mutex>

#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void snd(mutex * mtx)
{
	// Setting up client socket
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(22222);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	// Sending message
	char msg[1024];
	while(strcmp(msg, "quit")){
		cin.getline(msg, 1024);
		mtx->lock();
		cout << "snd > " << msg << endl;
		mtx->unlock();

		sendto(s, msg, 1024, 0, (sockaddr*) &addr, sizeof(addr));
	}
	// Closing socket
	closesocket(s);

}
void rcv(mutex * mtx)
{
	// Setting up server socket
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(22222);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	::bind(s, (sockaddr*) &addr, sizeof(addr));

	// recieving message
	char msg[1024];
	while(strcmp(msg, "quit")){
		int res = recv(s, msg, 1024, 0);

		mtx->lock();
		cout << "rcv < " << msg << endl;
		mtx->unlock();
	}
	// Closing socket
	closesocket(s);
}

int main(int argc, char ** argv)
{
	// Initializing WinSock
	WSAData wd;
	WSAStartup(MAKEWORD(2,0), &wd);

	mutex mtx;
	thread th0(snd, &mtx);
	thread th1(rcv, &mtx);

	th1.join();
	th0.join();

	// Destroying WinSock
	WSACleanup();

	return 0;
}
