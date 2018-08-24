#include <stdio.h>
#include "Global.h"
#include "Definition.h"
#include "action.h"
#include <string>
#include <stdlib.h>
#include <iostream>


void action ( char* returnmsg )
{

// test code begin

    Global::EQBaudrateReturn_cmd.IP = "192.168.0.90";

    for ( int i = 0; i < 11; i++ )
    {
        Global::EQBaudrateReturn_cmd.PortSetting[i] = 1;
    }

    printf ( "IP = %s\n", Global::EQBaudrateReturn_cmd.IP );
    char returnval[ReturnMsgBuffer];

    Global::EQDataReturn_cmd.IP = "192.168.0.90";
    Global::EQDataReturn_cmd.Data[0] = "@01-03-02-00-10-5F-A4";
    Global::EQDataReturn_cmd.Data[1] = "@01-03-02-00-10-5F-A4@02-03-02-00-10-5F-A4@03-03-02-00-10-5F-A4@04-03-02-00-10-5F-A4@05-03-02-00-10-5F-A4@06-03-02-00-10-5F-A4@07-03-02-00-10-5F-A4@08-03-02-00-10-5F-A4@09-03-02-00-10-5F-A4@10-03-02-00-10-5F-A4@11-03-02-00-10-5F-A4@01-03-02-00-10-5F-A4";
    Global::EQDataReturn_cmd.Data[2] = "@01-03-02-00-10-5F-A4";
    Global::EQDataReturn_cmd.Data[3] = "@01-03-02-00-10-5F-A4";
    Global::EQDataReturn_cmd.Data[4] = "@01-03-02-00-10-5F-A4";
    Global::EQDataReturn_cmd.Data[5] = "@01-03-02-00-10-5F-A4";
    Global::EQDataReturn_cmd.Data[6] = "@01-03-02-00-10-5F-A4";
    Global::EQDataReturn_cmd.Data[7] = "@01-03-02-00-10-5F-A4@02-03-02-00-10-5F-A4@03-03-02-00-10-5F-A4@04-03-02-00-10-5F-A4@05-03-02-00-10-5F-A4@06-03-02-00-10-5F-A4";
    Global::EQDataReturn_cmd.Data[8] = "@01-03-02-00-10-5F-A4@02-03-02-00-10-5F-A4@03-03-02-00-10-5F-A4@04-03-02-00-10-5F-A4@05-03-02-00-10-5F-A4@06-03-02-00-10-5F-A4";
    Global::EQDataReturn_cmd.Data[9] = "@01-03-02-00-10-5F-A4@02-03-02-00-10-5F-A4@03-03-02-00-10-5F-A4@04-03-02-00-10-5F-A4@05-03-02-00-10-5F-A4@06-03-02-00-10-5F-A4";
    Global::EQDataReturn_cmd.Data[10] = "@01-03-02-00-10-5F-A4@02-03-02-00-10-5F-A4@03-03-02-00-10-5F-A4@04-03-02-00-10-5F-A4@05-03-02-00-10-5F-A4@06-03-02-00-10-5F-A4";
    Global::EQDataReturn_cmd.Data[11] = "@01-03-02-00-10-5F-A4@02-03-02-00-10-5F-A4@03-03-02-00-10-5F-A4@04-03-02-00-10-5F-A4@05-03-02-00-10-5F-A4@06-03-02-00-10-5F-A4";


// end test code

    switch ( Global::RqtType )
    {
    case BaudrateRequest:
        Global::DataLatch = true;
        sprintf ( returnval, "EQBaudrate*%s#1,%d#2,%d#3,%d#4,%d#5,%d#6,%d#7,%d#8,%d#9,%d#10,%d#11,%d#12,%d*&", Global::EQBaudrateReturn_cmd.IP, Global::EQBaudrateReturn_cmd.PortSetting[0], Global::EQBaudrateReturn_cmd.PortSetting[1], Global::EQBaudrateReturn_cmd.PortSetting[2], Global::EQBaudrateReturn_cmd.PortSetting[3], Global::EQBaudrateReturn_cmd.PortSetting[4], Global::EQBaudrateReturn_cmd.PortSetting[5], Global::EQBaudrateReturn_cmd.PortSetting[6], Global::EQBaudrateReturn_cmd.PortSetting[7], Global::EQBaudrateReturn_cmd.PortSetting[8], Global::EQBaudrateReturn_cmd.PortSetting[9], Global::EQBaudrateReturn_cmd.PortSetting[10], Global::EQBaudrateReturn_cmd.PortSetting[11] );
        break;

    case IdRequest:
        Global::DataLatch = true;
        sprintf ( returnval, "EQid*%s#1,%s#2,%s#3,%s#4,%s#5,%s#6,%s#7,%s#8,%s#9,%s#10,%s#11,%s#12,%s*&", Global::EQIdReturn_cmd.IP, Global::EQIdReturn_cmd.PortID[0], Global::EQIdReturn_cmd.PortID[1], Global::EQIdReturn_cmd.PortID[2], Global::EQIdReturn_cmd.PortID[3], Global::EQIdReturn_cmd.PortID[4], Global::EQIdReturn_cmd.PortID[5], Global::EQIdReturn_cmd.PortID[6], Global::EQIdReturn_cmd.PortID[7], Global::EQIdReturn_cmd.PortID[8], Global::EQIdReturn_cmd.PortID[9], Global::EQIdReturn_cmd.PortID[10], Global::EQIdReturn_cmd.PortID[11] );
        break;

    case DataRequest:
        Global::DataLatch = true;
        sprintf ( returnval, "EQData*%s#1%s#2%s#3%s#4%s#5%s#6%s#7%s#8%s#9%s#10%s#11%s#12%s*&", Global::EQDataReturn_cmd.IP, Global::EQDataReturn_cmd.Data[0], Global::EQDataReturn_cmd.Data[1], Global::EQDataReturn_cmd.Data[2], Global::EQDataReturn_cmd.Data[3], Global::EQDataReturn_cmd.Data[4], Global::EQDataReturn_cmd.Data[5], Global::EQDataReturn_cmd.Data[6], Global::EQDataReturn_cmd.Data[7], Global::EQDataReturn_cmd.Data[8], Global::EQDataReturn_cmd.Data[9], Global::EQDataReturn_cmd.Data[10], Global::EQDataReturn_cmd.Data[11] );
        break;
    }

    Global::DataLatch = false;

    for ( int i = 0; i < ReturnMsgBuffer; i++ )
    {
        returnmsg[i] = returnval[i];
    }


    return;
}
