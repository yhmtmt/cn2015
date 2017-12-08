#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char ** argv){
  // Setting up client socket
  int s = socket(AF_INET, SOCK_DGRAM, 0);
  
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(22222);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  // Sending message
  char msg[1024] = "Hello world";
  
  sendto(s, msg, strlen(msg)+1, 0, (sockaddr*) &addr, sizeof(addr));
  
  // Closing socket
  close(s);
  
  return 0;
}
