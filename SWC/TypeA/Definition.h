#ifndef DEFINITION_H
#define DEFINITION_H

#include <string.h>
#include <string>

using namespace std;

#define EndCahr "0x04"
#define ReturnMsgBuffer 2048

enum RequestType{BaudrateRequest = 0, IdRequest = 1, DataRequest = 2};

typedef struct
{
   int port[12];
   int baurate[12];
   char *dataformat[12];

}EQBaudrate;


typedef struct
{
   char *IP;
   int PortSetting[12];
}EQBaudrateReturn;

typedef struct
{
   char *IP;
   char *ID[12];
}EQId;

typedef struct
{
   char *IP;
   char *PortID[12];  //if none, return FF
}EQIdReturn;


enum {KCM = 0, HUSKY = 1, SDC15 = 2};

typedef struct
{
   char *CmdType[3];
   int  CmdNum[3];
   int PortType[12];
}EQType;


typedef struct
{
   char *KCMCmd[12];
   char *HUSKYCmd[12];
   char *SDC15Cmd[12];
}EQCmd;

typedef struct
{
   int CmdNum;
}EQData;

typedef struct
{
   char *IP;
   char *PortID[12];  
   char *Data[12];     //if none, return FF
}EQDataReturn;

#endif
