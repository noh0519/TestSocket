#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF_SIZE 1024

int main(void)
{
   int server_socket;
   int client_socket;

   struct sockaddr_in server_addr;
   struct sockaddr_in client_addr;
   socklen_t client_addr_size;

   char buff_rcv[BUFF_SIZE + 5];
   char buff_snd[BUFF_SIZE + 5];

   server_socket = socket(PF_INET, SOCK_STREAM, 0);
   if (-1 == server_socket)
   {
      printf("socket 생성 실패\n");
      exit(1);
   }

   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(4000);
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

   if (-1 == bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)))
   {
      printf("bind 실패\n");
      exit(1);
   }

   while (1)
   {
      if (-1 == listen(server_socket, 5))
      {
         printf("listen 모드 설정 실패\n");
         exit(1);
      }

      client_addr_size = sizeof(client_addr);
      /***client 접속 요청이 올때 까지 여기에서 block 상태로 대기***/
      client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);

      if (-1 == client_socket)
      {
         printf("client 연결 실패\n");
         exit(1);
      }

      read(client_socket, buff_rcv, BUFF_SIZE);
      printf("receive: %s\n", buff_rcv);

      sprintf(buff_snd, "%d : %s", strlen(buff_rcv), buff_rcv);
      write(client_socket, buff_snd, strlen(buff_snd) + 1); // +1 : NULL까지 포함해서 전송
      close(client_socket);
   }
}