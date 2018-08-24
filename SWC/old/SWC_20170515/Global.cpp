#include "Global.h"
#include "Definition.h"

int Global::SettingProcedure = 0;
bool Global::DataLatch = false;
bool Global::CommunicationOpened = false;

RequestType Global::RqtType = BaudrateRequest;
EQBaudrate Global::EQBaudrate_cmd = {};
EQId Global::EQId_cmd = {};
EQType Global::EQType_cmd = {};
EQCmd Global::EQCmd_cmd = {};
EQData Global::EQData_cmd = {};

EQBaudrateReturn Global::EQBaudrateReturn_cmd = {};
EQIdReturn Global::EQIdReturn_cmd = {};
EQDataReturn Global::EQDataReturn_cmd = {};

