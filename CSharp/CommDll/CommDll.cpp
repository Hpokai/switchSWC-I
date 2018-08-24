// CommDll.cpp : 定義 DLL 應用程式的匯出函式。
//

#include "stdafx.h"
#include "CommDll.h"
#include "ComLib.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

namespace CommDll{
	extern "C" int DLLEXPORT Connection(const char *strIP, const char *strPort)
	{
		int val = 1;
		CreateConnect(strIP, strPort);
		return val;
	}

	extern "C" int DLLEXPORT MsgSend(const char *str)
	{
		int val = 1;
		SendMsg(str);
		return val;
	}

	extern "C" DLLEXPORT LPSTR MsgReceive()
	{		
//		FILE *fp;
		string msg = ReceiveMsg();
		LPSTR szName;

//		szName = (char *) (msg.c_str());

		szName = new char[msg.length() + 1];
		strcpy(szName, msg.c_str());

/*		fp = fopen("output.txt","w");
		fprintf(fp,"%s\n",szName);
		fclose(fp);
	*/	
		return szName;
	}

	extern "C" int DLLEXPORT DisConnection()
	{
		int val = 1;
		StopConnect();
		return val;
	}
}


