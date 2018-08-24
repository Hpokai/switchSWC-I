#ifndef GLOBAL_H
#define GLOBAL_H

#include "Definition.h"

class Global
{
public:
    static int SettingProcedure;
    static bool CommunicationOpened;
    static bool isDataReady;
    static int isSubDataReady[MAX_CONNECTOR];
    static bool isFirstRoutine_DataCapture[MAX_CONNECTOR];
    static bool DataLatch_RoutineEqData;        //
    static bool DataLatch_RoutineEqId;          //
    static int isRoutineDataReady[MAX_CONNECTOR];
    static int isRoutineIdReady[MAX_CONNECTOR];
    static bool isIdExist[MAX_CONNECTOR];

    static bool isDataUpdate[MAX_CONNECTOR];
    static bool isProcessUpdate[MAX_CONNECTOR];
    static bool isActionUpdate;
    static bool isExitThread;

    static bool isRoutineIdUpdate[MAX_CONNECTOR];
    static bool isRoutineEqUpdate[MAX_CONNECTOR];
    static bool isProcessIdUpdate[MAX_CONNECTOR];
    static bool isProcessEqUpdate[MAX_CONNECTOR];

    static RequestType RqtType;
    static EQBaudrate EQBaudrate_cmd;
    static EQId       EQId_cmd;
    static EQType     EQType_cmd;
    static EQCmd      EQCmd_cmd;
    static EQData     EQData_cmd;
    static EQIdReturn EQIdReturn_cmd;
    static EQBaudrateReturn EQBaudrateReturn_cmd;
    static EQDataReturn EQDataReturn_cmd;

    static EQIdReturn temp_EQIdReturn_cmd;
    static EQBaudrateReturn temp_EQBaudrateReturn_cmd;
    static EQDataReturn temp_EQDataReturn_cmd;

    static MainState enum_MainState;
    static SettingState enum_SettingState;
    static RoutineState enum_RoutineState;
};


#endif
