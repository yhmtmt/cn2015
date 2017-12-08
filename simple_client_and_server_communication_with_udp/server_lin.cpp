#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char ** argv)
{
  // Setting up server socket
  int s = socket(AF_INET, SOCK_DGRAM, 0);
  
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(22222);
  addr.sin_addr.s_addr = INADDR_ANY;

  bind(s, (sockaddr*) &addr, sizeof(addr));
  
  // recieving message
  char msg[1024];
  recv(s, msg, sizeof(msg), 0);
  printf("%s\n", msg);
  
  // Closing socket
  close(s);
  
  return 0;
}
