#ifndef DEFINITION_H
#define DEFINITION_H

#include <string.h>
#include <string>

using namespace std;

#define EndCahr "0x04"
#define ReturnMsgBuffer 2048

#define DataNotReady 0
#define DataReady 1
#define DataGotten 2

#define MAX_CONNECTOR 12
#define BUFFER_SIZE 32

 enum RequestType {BaudrateRequest = 0, SIdRequest = 1, DataRequest = 2, IdRequest = 3, None = 4};
 enum MainState {Main_None = 0, Main_Setting = 1, Main_Routine = 2};
 enum SettingState {Setting_None = 0, Setting_EQId = 1, Setting_EQBaudrate = 2, Setting_EQType = 4, Setting_EQCmd = 8};
 enum RoutineState {Routine_None = 0, Routine_EQId = 1, Routine_EQData = 2,Routine_EQReset = 3};

typedef struct
{
    int port[MAX_CONNECTOR];
    int baurate[MAX_CONNECTOR];
    char *dataformat[MAX_CONNECTOR];

} EQBaudrate;


typedef struct
{
    char *IP;
    int PortSetting[MAX_CONNECTOR];
} EQBaudrateReturn;

typedef struct
{
    char *IP;
    char *ID[MAX_CONNECTOR];
} EQId;

typedef struct
{
    char *IP;
    char *PortID[MAX_CONNECTOR];  //if none, return FF
} EQIdReturn;


enum {NoEQ = -1, KCM = 0, HUSKY = 1, SDC15 = 2};

typedef struct
{
    char *CmdType[3];
    int  CmdNum[3];
    int PortType[MAX_CONNECTOR];
} EQType;


typedef struct
{
    char *KCMCmd[MAX_CONNECTOR];
    char *HUSKYCmd[MAX_CONNECTOR];
    char *SDC15Cmd[MAX_CONNECTOR];
} EQCmd;

typedef struct
{
    int CmdNum;
} EQData;

typedef struct
{
    char *IP;
    //char *PortID[MAX_CONNECTOR];
    char *Data[MAX_CONNECTOR];     //if none, return FF
} EQDataReturn;

#endif
