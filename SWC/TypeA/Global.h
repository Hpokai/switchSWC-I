#ifndef GLOBAL_H
#define GLOBAL_H

#include "Definition.h"

class Global{
public:
  static int SettingProcedure;
  static bool DataLatch;
  static bool CommunicationOpened;
  static RequestType RqtType;
  static EQBaudrate EQBaudrate_cmd;
  static EQId       EQId_cmd;
  static EQType     EQType_cmd;
  static EQCmd      EQCmd_cmd;
  static EQData     EQData_cmd;
  static EQBaudrateReturn EQBaudrateReturn_cmd;
  static EQIdReturn EQIdReturn_cmd;
  static EQDataReturn EQDataReturn_cmd;
};


#endif
