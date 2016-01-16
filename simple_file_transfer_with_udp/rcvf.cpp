#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

// * The program recieves a file from the client (sndf.cpp).
// * The program assumes following protcol.
//   1. Recieve the first packet includes, file name, packet size, number of packets
//	 2. Recieve data packets  
// * The first packet = | number of packets |		file name	    |
// * The data packet =  | data size         |		data	        |
// * Note that the packet size should be the same as that specified in the client.


struct packet
{
	int szdata;
	char * data;
};

int main(int argc, char ** argv)
{
	if(argc != 3){
		printf("Usage: sndf <port number> <packet size>\n");
		return 0;
	}

	// Translating port number
	int nport = atoi(argv[1]);

	// Translating packet size 
	int szpkt = atoi(argv[2]);

	if(szpkt <= sizeof(int)){
		printf("Packet size must be larger than %d. (%d specified)\n", sizeof(int), szpkt);
		return 0;
	}

	// Initializing WinSock
	WSAData wd;
	WSAStartup(MAKEWORD(2,0), &wd);

	// Setting up server socket
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons((u_short) nport);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(s, (sockaddr*) &addr, sizeof(addr));


	// preparing recieving data buffer.
	char * buf = (char*) malloc((size_t) szpkt);

	// Recieving first packet
	recv(s, buf, szpkt, 0);

	int npkts = *((int*) buf);
	char * fname = (char*) malloc(strlen(buf + sizeof(npkts)) + 1); 
	strcpy(fname, buf + sizeof(npkts));

	printf("File %s (%d packets) recieving.", fname, npkts);

	// Allocating and initializing packet storage
	packet * pkts = (packet*) malloc(sizeof(packet) * npkts);
	if(pkts == NULL){
		printf("Failed to allocate packet's storage.\n");
		exit(1);
	}

	memset((void*)pkts, 0, sizeof(packet) * npkts);
	
	// recieving message
	for(int i = 0; i < npkts; i++){
		recv(s, buf, szpkt, 0);
		int idx = *((int*) buf);
		pkts[idx].szdata = *((int*) (buf + sizeof(int)));
		pkts[idx].data = (char*) malloc(pkts[idx].szdata);
		memcpy((void*) pkts[idx].data, (void*) (buf + sizeof(int) * 2), pkts[idx].szdata);
		printf("%dth packet recieved.\n", idx);
	}

	// write file
	FILE * pf = fopen(fname, "wb");
	for (int i = 0; i < npkts; i++){
		if(i != npkts - 1 && pkts[i].szdata == 0){
			printf("%dth packet is not recieved.", i);
			continue;
		}
		fwrite((void*) pkts[i].data, sizeof(char), pkts[i].szdata, pf);
		free(pkts[i].data);
	}
	fclose(pf);
	free(pkts);

	free(fname);
	free(buf);

	// Closing socket
	closesocket(s);

	// Destroying WinSock
	WSACleanup();

	return 0;
}
