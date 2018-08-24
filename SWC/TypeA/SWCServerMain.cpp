
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <pthread.h>
#include "interpreter.h"
#include "Global.h"
#include "action.h"
#include "DataCapture.h"
#include "Communication.h"

using namespace std;

int main()
{
 int sockfd;
 struct sockaddr_in dest;
 char buffer[2048] = "Hi Client!";
 char buffer1[2048];
 
 pthread_t threads[1];
;
 int ret;

 sockfd = socket(PF_INET, SOCK_STREAM, 0);

 bzero(&dest, sizeof(dest));
 dest.sin_family = AF_INET;
 dest.sin_port = htons(5000);
 dest.sin_addr.s_addr = INADDR_ANY;

 bind(sockfd, (struct sockaddr*)&dest, sizeof(dest));
 listen(sockfd,20);

 int clientfd;
 struct sockaddr_in client_addr;
 int interpreterloopcomplete = 0;
 socklen_t addrlen = sizeof(client_addr);

 clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);

//  send(clientfd, buffer, sizeof(buffer),0);
 bool ThreadCreated = false;

 while(1)
 {
  memset(buffer1,0,100);
  memset(buffer,0,100);

  int res = recv(clientfd, buffer1, sizeof(buffer1),0);  

  interpreterloopcomplete = interpreter(buffer1,res);

  if(((Global::SettingProcedure & 15) == 15)&&(!ThreadCreated)){
    ThreadCreated = true;
    ret = pthread_create(&threads[0],NULL,DataCapture,(void *) 1);
    if(ret!=0){
      printf("Create pthread error!\n");
    }
  }
  action(buffer);
  printf("%s\n","pppppppppp");

  send(clientfd, buffer, sizeof(buffer),0);

  memset(buffer1,0,100);
  memset(buffer,0,100);

//  strcpy(buffer,buffer1);
//  memset(buffer,0,100);
//  send(clientfd, buffer, sizeof(buffer),0);
//  close(clientfd);

 }
 pthread_exit(NULL);
 close(clientfd);
 close(sockfd);
 return 0;
}
