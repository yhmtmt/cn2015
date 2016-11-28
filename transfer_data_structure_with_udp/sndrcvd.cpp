
#include <iostream>
#include <thread>
#include <mutex>

#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct data {
	int a;
	float b;
	double c;
};

void snd(mutex * mtx)
{
	// Setting up client socket
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(22222);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	// Sending data
	data d;
	d.a = 100;
	d.b = 12.5;
	d.c = 12.5555555;

	mtx->lock();
	cout << "snd > " << d.a << " " << d.b << " " << d.c << endl;
	mtx->unlock();

	sendto(s, (char*)&d, sizeof(d), 0, (sockaddr*) &addr, sizeof(addr));

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

	// recieving data
	data d;
	int res = recv(s, (char*)&d, sizeof(d), 0);

	mtx->lock();
	cout << "rcv < " << d.a << " " << d.b << " " << d.c << endl;
	mtx->unlock();

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
