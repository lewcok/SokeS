//
//  main.c
//  sockC
//
//  Created by laukevin on 14-10-23.
//  Copyright (c) 2014年 laukevin. All rights reserved.
//

//
//  main.m
//  server
//
//  Created by mac on 13-4-15.
//  Copyright (c) 2013年 mac. All rights reserved.
//

//#import <Foundation/Foundation.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int CreateServerSocket(short port)
{
    int socket_server=0;
    socket_server=socket(AF_INET,SOCK_STREAM,0);
    //填充IP与端口
    /*
     struct sockaddr_in {
     __uint8_t  sin_len;
     sa_family_t    sin_family;
     in_port_t  sin_port;
     struct in_addr sin_addr;
     char       sin_zero[8];
     };
     */
    struct sockaddr_in sAddr = {0};
    sAddr.sin_len=sizeof(sAddr);
    sAddr.sin_family=AF_INET;
    sAddr.sin_port= htons(port); //本机字节序转网络字节序
    sAddr.sin_addr.s_addr=INADDR_ANY;
    //绑定  //(struct sockaddr *)&sAddr强转
    if (bind(socket_server, (struct sockaddr *)&sAddr, sizeof(sAddr)) != 0) {
        return 0;
    }
    //监听
    if(listen(socket_server, 100) != 0){
        return 0;
    }
    return socket_server;
}
//接收连接并返回客户端的scoket
int AcceptClientSocket(int ServerSocket)
{
    int ClinetSocket=0;
    struct sockaddr_in cAddr = {0};
    socklen_t len=sizeof(cAddr);
    ClinetSocket = accept(ServerSocket, (struct sockaddr *)&cAddr, &len);
    char * ipaddress=inet_ntoa(cAddr.sin_addr);
    printf("accept--->%s,%d\n",ipaddress,cAddr.sin_port);
    return ClinetSocket;
}
void Message(int socket)
{
    char send_Message[1024]="hello\n";
    char recv_Message[1024]={0};
    send(socket, send_Message, strlen(send_Message)+1, 0);
    while (1) {
        recv(socket, recv_Message, sizeof(recv_Message), 0);
        printf("<-----%s",recv_Message);
        printf("----->");
        fgets(send_Message, sizeof(send_Message), stdin);
        send(socket,send_Message,strlen(send_Message)+1,0);
    }
}

int main(int argc, const char * argv[])
{
    short port=9897;
    if (argc>1) {
        port=(short)atoi(argv[1]);
    }
    int ServerSocket = CreateServerSocket(port);
    if (ServerSocket == 0) {
        printf("Cerate scoket error\n");
        return 0;
    }
    printf("Cerate scoket ok!\n");
    
    while (1)
    {
        int ClinetScoket=AcceptClientSocket(ServerSocket);
        if (ClinetScoket == 0)
        {
            printf("Client connect error\n");
            return 0;
        }
        printf("Client connect ok.....\n");
        Message(ClinetScoket);
        return 0;
    }
    
    //    //接收连接
    ////    accept(<#int#>, <#struct sockaddr *restrict#>, <#socklen_t *restrict#>)
    //    struct sockaddr sAdd2={0};
    //    socklen_t len=sizeof(struct sockaddr);
    //    accept(ServerSocket, &sAdd2, &len);
    
    return 0;
}
