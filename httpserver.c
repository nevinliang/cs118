#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define LISTEN_BACKLOG 50

int main(int argc, char *argv[]) {

  int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in sockin;

  memset(&sockin, 0, sizeof(sockin));
  sockin.sin_family = AF_INET;
  sockin.sin_port = htons(8123);
  
  struct in_addr internet_address;
  internet_address.s_addr = htonl(INADDR_LOOPBACK);
  
  sockin.sin_addr = internet_address;
  
  
  bind(tcp_socket, (struct sockaddr *) &sockin, sizeof(sockin));
  
  listen(tcp_socket, LISTEN_BACKLOG);

  int cfd = accept(tcp_socket, NULL, NULL);

  char buf[100];
  read(cfd, buf, 100);

  int nums[2];
  int mux = 0;
  for (int i = 0; i < 100; ) {
    if (buf[i] == '=') {
      i++;
      while (buf[i] >= '0' && buf[i] <= '9') {
        nums[mux] *= 10;
        nums[mux] += buf[i] - '0';
        i++;
      }
      mux++;
    }
    i++;
  }
  int sum = nums[0] + nums[1];
  char obuf[5];
  int outputsize = sprintf(obuf, "%d\n", sum);

  char ibuf[] = "HTTP/1.0 200 OK\n\n";
  write(cfd, ibuf, sizeof(ibuf));
  write(cfd, obuf, outputsize);


  close(tcp_socket);
  close(cfd);
}
