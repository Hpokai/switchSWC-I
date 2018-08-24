
#pragma once

#ifndef COMMDLL_H
#define COMMDLL_H
#define DLLEXPORT __declspec(dllexport) 

#include <stdio.h>
#include <string>

using namespace std;

extern "C"{
	DLLEXPORT int Connection();
	DLLEXPORT int MsgSend(const char *str);
	DLLEXPORT LPSTR MsgReceive();
	DLLEXPORT int DisConnection();
}
   
#endif