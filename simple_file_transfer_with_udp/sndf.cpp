#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

// * The program sends a file to the server (rcvf.cpp).
// * The program assumes following protcol.
//   1. sends first packet includes, file name, packet size, number of packets
//	 2. sends data packets  
// * The first packet = | number of packets |		file name	    |
// * The data packet =  | index | data size |		data	        |
// * Note that the packet size should be the same as that specified in the server.

int main(int argc, char ** argv)
{
	if(argc != 5){
		printf("Usage: sndf <file name> <destination IP address> <port number> <packet size>\n");
		return 0;
	}

	// Translating port number
	int nport = atoi(argv[3]);

	// Translating packet size 
	int szpkt = atoi(argv[4]);

	if(szpkt <= sizeof(int) * 2){
		printf("Packet size must be larger than %d. (%d specified)\n", sizeof(int) * 2, szpkt);
		return 0;
	}

	if(strlen(argv[1]) + 1 > szpkt - sizeof(int)){
		printf("Packet size must be larger enough to send file name %s.", argv[1]);
		return 0;
	}

	// Initializing WinSock
	WSAData wd;
	WSAStartup(MAKEWORD(2,0), &wd);

	// Setting up socket
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons((u_short) nport);
	addr.sin_addr.S_un.S_addr = inet_addr(argv[2]);

	// Opening file and getting file size
	FILE * pf = fopen(argv[1], "rb" /* read and binary mode */); 
	int szf;
	fseek(pf, 0, SEEK_END); // moving file pointer to the end,
	szf = ftell(pf);		// The address should be the file size.
	fseek(pf, 0, SEEK_SET);		// rewind the pointer to the head.

	// preparing file transmission
	char * buf = (char*) malloc((size_t) szpkt);	// allocating data buffer to be sent
	int szdata = szpkt - sizeof(int) * 2;				// size of the packet
	int npkts = (int) szf / szdata + 1;				// number of packets

	printf("File %s, %d packets, Packet Size %d, %d bytes\n", argv[1], npkts, szpkt, szf);

	// preparing and sending first packet
	memcpy((void*) buf, (void*)&npkts, sizeof(npkts));
	memcpy((void*) (buf + sizeof(npkts)), (void*)argv[1], strlen(argv[1]) + 1);
	sendto(s, buf, szpkt, 0, (sockaddr*) &addr, sizeof(addr));

	// sending all data packets
	for(int i = 0; i < npkts; i++){
		Sleep(100);
		memcpy((void*) buf, (void*) &i, sizeof(int));
		if(i == npkts - 1){			
			szdata = szf % (szpkt - sizeof(int) * 2);
		}
		memcpy((void*) (buf + sizeof(int)), (void*) &szdata, sizeof(int));
		fread((void*)(buf + sizeof(int) * 2), sizeof(char), (size_t) szdata, pf);
		sendto(s, buf, szpkt, 0, (sockaddr*) &addr, sizeof(addr));
		printf("%dth packet sent.\n", i);	
	}

	free(buf);
	fclose(pf);

	// Closing socket
	closesocket(s);

	// Destroying WinSock
	WSACleanup();
	return 0;
}
