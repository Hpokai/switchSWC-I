#include <stdio.h>
#include <stdlib.h>

#include "Global.h"
#include "Definition.h"

int Global::SettingProcedure = 0;
bool Global::isCommunicationOpened[MAX_CONNECTOR] = {false};


int Global::RecvDataTimeStep = 0;
bool Global::isEQSID = false;

RequestType Global::RqtType = None;

MainState Global::enum_MainState = Main_None;
SettingState Global::enum_SettingState = Setting_None;
RoutineState Global::enum_RoutineState = Routine_None;
bool Global::isDataReady = false;
int Global::isSubDataReady[MAX_CONNECTOR] =
{
    DataNotReady, DataGotten, DataGotten, DataGotten, DataGotten, DataGotten,
    DataGotten, DataGotten, DataGotten, DataGotten, DataGotten, DataGotten
};


bool Global::isFirstRoutine_DataCapture[MAX_CONNECTOR] =
{
    false, false, false, false, false, false,
    false, false, false, false, false, false
};
bool Global::DataLatch_RoutineEqData = false;
bool Global::DataLatch_RoutineEqId = false;


int Global::isRoutineDataReady[MAX_CONNECTOR] =
{
    DataNotReady, DataGotten, DataGotten, DataGotten, DataGotten, DataGotten,
    DataGotten, DataGotten, DataGotten, DataGotten, DataGotten, DataGotten
};

int Global::isRoutineIdReady[MAX_CONNECTOR] =
{
    DataNotReady, DataGotten, DataGotten, DataGotten, DataGotten, DataGotten,
    DataGotten, DataGotten, DataGotten, DataGotten, DataGotten, DataGotten
};

int Global::HardwareThereCount[MAX_CONNECTOR] =
{
    DataNotReady, DataGotten, DataGotten, DataGotten, DataGotten, DataGotten,
    DataGotten, DataGotten, DataGotten, DataGotten, DataGotten, DataGotten
};

bool Global::isIdExist[MAX_CONNECTOR] =
{
    false, false, false, false, false, false,
    false, false, false, false, false, false
};
bool Global::isDataUpdate[MAX_CONNECTOR] = {false};
bool Global::isProcessUpdate[MAX_CONNECTOR] = {false};
bool Global::isActionUpdate = false;
bool Global::isExitThread = false;

bool Global::isRoutineIdUpdate[MAX_CONNECTOR] = {false};
bool Global::isRoutineEqUpdate[MAX_CONNECTOR] = {false};
bool Global::isProcessIdUpdate[MAX_CONNECTOR] = {false};
bool Global::isProcessEqUpdate[MAX_CONNECTOR] = {false};

bool Global::isHardwareThere[MAX_CONNECTOR] = {false};


EQBaudrate Global::EQBaudrate_cmd =
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // num = 12
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // num = 12
    {
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) )
    }
};
EQId Global::EQId_cmd =
{
    ( char* ) malloc ( 20 * sizeof ( char ) ),
    {
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) )
    }
};
EQType Global::EQType_cmd =
{
    {
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) ),
        ( char* ) malloc ( 8 * sizeof ( char ) )
    },
    {0, 0, 0},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};
EQCmd Global::EQCmd_cmd =
{
    {
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) )
    },
    {
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) )
    },
    {
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) ),
        ( char* ) malloc ( 32 * sizeof ( char ) )
    }
};
EQData Global::EQData_cmd =
{
    0
};

EQBaudrateReturn Global::EQBaudrateReturn_cmd =
{
    ( char* ) malloc ( 20 * sizeof ( char ) ),
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
EQIdReturn Global::EQIdReturn_cmd =
{
    ( char* ) malloc ( 20 * sizeof ( char ) ),
    {
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) )
    }
};
EQDataReturn Global::EQDataReturn_cmd =
{
    ( char* ) malloc ( 20 * sizeof ( char ) ),
    {
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) )
    }
};

EQIdReturn Global::temp_EQIdReturn_cmd =
{
    ( char* ) malloc ( 20 * sizeof ( char ) ),
    {
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) ),
        ( char* ) malloc ( 20 * sizeof ( char ) )
    }
};
EQBaudrateReturn Global::temp_EQBaudrateReturn_cmd =
{
    ( char* ) malloc ( 20 * sizeof ( char ) ),
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
EQDataReturn Global::temp_EQDataReturn_cmd =
{
    ( char* ) malloc ( 20 * sizeof ( char ) ),
    {
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) ),
        ( char* ) malloc ( 2048 * sizeof ( char ) )
    }
};



