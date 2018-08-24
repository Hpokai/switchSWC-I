
#include "stdafx.h"

#include "ComLib.h"

#define	BUF_SZIE	2048
#include "winsock2.h"

#include <cstdlib>
#include <iostream>
#include <fstream> 
#include <stdio.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

static WSADATA	wsd;
static SOCKET sHost;			
static SOCKADDR_IN	servAddr;		
static char buf[BUF_SZIE];	
static char buf1[BUF_SZIE];	
static int retVal;



bool CreateConnect(string IP, string Port)
{
	bool val = true;
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
	{
		printf("WSAStartup failed!\n");
		val = false;
		return val;
	}
	
	
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	
	if(INVALID_SOCKET == sHost)
	{
		printf("socket failed!\n");
		WSACleanup();
		val = false;
		return val;
	}

	LPSTR IPName = new char[IP.length() + 1];
	strcpy(IPName, IP.c_str());
	int IPNum = atoi( Port.c_str() );

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(IPName);
	servAddr.sin_port = htons((short) IPNum);
	int	nServAddlen  = sizeof(servAddr);

	retVal=connect(sHost,(LPSOCKADDR)&servAddr, sizeof(servAddr));	
	if(SOCKET_ERROR == retVal)
	{
		printf("connect failed!\n");	
		closesocket(sHost);	
		WSACleanup();		
		val = false;
		return val;
	}
	
	return val;
}

bool SendMsg(string str)
{	
	bool val = true;
	ZeroMemory(buf, BUF_SZIE);

	strcpy_s(buf, str.c_str());
	int size = sizeof(str.c_str());

	size = strlen(str.c_str());

	retVal = send(sHost, buf, size, 0);

	if (SOCKET_ERROR == retVal)
	{
		printf("send failed!\n");
		val = false;
		closesocket(sHost);	
		WSACleanup();		
		return val;
	}
	return val;
}

string ReceiveMsg(){
	bool val = true;
	FILE *fp;
	string rmsg="";
	ZeroMemory(buf1, BUF_SZIE);
	retVal = recv(sHost, buf1, BUF_SZIE, 0);
	

	if (SOCKET_ERROR == retVal)
	{
		val = false;
		printf("recv failed!\n");		
		closesocket(sHost);	
		WSACleanup();		
		return "Error";
	}

	rmsg = buf1;
	fp = fopen("abc.txt","w");
	fprintf(fp,buf1);
	fclose(fp);

	return rmsg;
}

bool StopConnect(){
    bool val = true;
	closesocket(sHost);	
	WSACleanup();	
	return val;
}
