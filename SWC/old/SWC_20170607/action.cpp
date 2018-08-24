#include <stdio.h>
#include "Global.h"
#include "Definition.h"
#include "action.h"
#include <string>
#include <stdlib.h>
#include <iostream>

char routineID[MAX_CONNECTOR][16] = {"FF"};
char routineDATA[MAX_CONNECTOR][600] = {"FF"};

void action ( char* returnmsg )
{
    // test code begin
    Global::EQIdReturn_cmd.IP = Global::EQId_cmd.IP;
    //Global::EQIdReturn_cmd.PortID[0] = "SN00021";
    Global::EQIdReturn_cmd.PortID[1] = "SN00005";
    Global::EQIdReturn_cmd.PortID[2] = "FF";
    Global::EQIdReturn_cmd.PortID[3] = "SN00003";
    Global::EQIdReturn_cmd.PortID[4] = "SN00100";
    Global::EQIdReturn_cmd.PortID[5] = "FF";
    Global::EQIdReturn_cmd.PortID[6] = "FF";
    Global::EQIdReturn_cmd.PortID[7] = "SN00110";
    Global::EQIdReturn_cmd.PortID[8] = "SN00201";
    Global::EQIdReturn_cmd.PortID[9] = "SN00111";
    Global::EQIdReturn_cmd.PortID[10] = "SN00006";
    Global::EQIdReturn_cmd.PortID[11] = "SN00007";

    Global::EQBaudrateReturn_cmd.IP = Global::EQId_cmd.IP;
    //Global::EQBaudrateReturn_cmd.PortSetting[0] = 1;
    Global::EQBaudrateReturn_cmd.PortSetting[1] = 1;
    Global::EQBaudrateReturn_cmd.PortSetting[2] = 1;
    Global::EQBaudrateReturn_cmd.PortSetting[3] = 0;
    Global::EQBaudrateReturn_cmd.PortSetting[4] = 0;
    Global::EQBaudrateReturn_cmd.PortSetting[5] = 1;
    Global::EQBaudrateReturn_cmd.PortSetting[6] = 1;
    Global::EQBaudrateReturn_cmd.PortSetting[7] = 0;
    Global::EQBaudrateReturn_cmd.PortSetting[8] = 1;
    Global::EQBaudrateReturn_cmd.PortSetting[9] = 1;
    Global::EQBaudrateReturn_cmd.PortSetting[10] = 1;
    Global::EQBaudrateReturn_cmd.PortSetting[11] = 0;

    Global::EQDataReturn_cmd.IP = Global::EQId_cmd.IP;
    //Global::EQDataReturn_cmd.Data[0] = "@01-03-02-03-01-79-74";
    Global::EQDataReturn_cmd.Data[1] = "@01-03-04-03-01-00-05-6B-B4";
    Global::EQDataReturn_cmd.Data[2] = "";
    Global::EQDataReturn_cmd.Data[3] = "@01-03-04-03-01-00-05-6B-B4";
    Global::EQDataReturn_cmd.Data[4] = "@01-03-04-03-01-00-05-6B-B4";
    Global::EQDataReturn_cmd.Data[5] = "";
    Global::EQDataReturn_cmd.Data[6] = "";
    Global::EQDataReturn_cmd.Data[7] = "@01-03-04-03-01-00-05-6B-B4";
    Global::EQDataReturn_cmd.Data[8] = "@10-01-26-20-3C-20-20-20-10-02-43-D5-00-00-10-03-8C-64";
    Global::EQDataReturn_cmd.Data[9] = "@01-03-04-03-01-00-05-6B-B4";
    Global::EQDataReturn_cmd.Data[10] = "@01-03-04-03-01-00-05-6B-B4";
    Global::EQDataReturn_cmd.Data[11] = "@10-01-26-20-3C-20-20-20-10-02-43-D5-00-00-10-03-8C-64";
// end test code

    char returnval[ReturnMsgBuffer] = {'\0'};

    if ( Main_Setting == Global::enum_MainState )
    {
        switch ( Global::RqtType )
        {
        case BaudrateRequest:
            sprintf ( returnval, "EQBaudrate*%s#1,%d#2,%d#3,%d#4,%d#5,%d#6,%d#7,%d#8,%d#9,%d#10,%d#11,%d#12,%d*&", Global::EQBaudrateReturn_cmd.IP,
                      Global::EQBaudrateReturn_cmd.PortSetting[0], Global::EQBaudrateReturn_cmd.PortSetting[1], Global::EQBaudrateReturn_cmd.PortSetting[2],
                      Global::EQBaudrateReturn_cmd.PortSetting[3], Global::EQBaudrateReturn_cmd.PortSetting[4], Global::EQBaudrateReturn_cmd.PortSetting[5],
                      Global::EQBaudrateReturn_cmd.PortSetting[6], Global::EQBaudrateReturn_cmd.PortSetting[7], Global::EQBaudrateReturn_cmd.PortSetting[8],
                      Global::EQBaudrateReturn_cmd.PortSetting[9], Global::EQBaudrateReturn_cmd.PortSetting[10], Global::EQBaudrateReturn_cmd.PortSetting[11] );
            break;

        case IdRequest:
            sprintf ( returnval, "EQId*%s#1,%s#2,%s#3,%s#4,%s#5,%s#6,%s#7,%s#8,%s#9,%s#10,%s#11,%s#12,%s*&", Global::EQIdReturn_cmd.IP,
                      Global::EQIdReturn_cmd.PortID[0], Global::EQIdReturn_cmd.PortID[1], Global::EQIdReturn_cmd.PortID[2],
                      Global::EQIdReturn_cmd.PortID[3], Global::EQIdReturn_cmd.PortID[4], Global::EQIdReturn_cmd.PortID[5],
                      Global::EQIdReturn_cmd.PortID[6], Global::EQIdReturn_cmd.PortID[7], Global::EQIdReturn_cmd.PortID[8],
                      Global::EQIdReturn_cmd.PortID[9], Global::EQIdReturn_cmd.PortID[10], Global::EQIdReturn_cmd.PortID[11] );
            break;

        default:
            if ( Global::enum_SettingState == Setting_EQType )
            {
                sprintf ( returnval, "Done_EQType" );
            }
            else
            {
                sprintf ( returnval, "Done_EQCmd" );
            }

            break;
        }
    }
    else
    {
        Global::isActionUpdate = true;

        for ( int i = 0; i < MAX_CONNECTOR; i++ )
        {
            switch ( Global::RqtType )
            {
            case IdRequest:
                if ( !Global::isProcessIdUpdate[i] )
                {
                    sprintf ( routineID[i], "%s", Global::EQIdReturn_cmd.PortID[i] );
                }

                break;

            case DataRequest:
                if ( !Global::isProcessEqUpdate[i] )
                {
                    sprintf ( routineDATA[i], "%s", Global::EQDataReturn_cmd.Data[i] );

                    printf("DataRequest!!!!!!!!!!!!!!!!!!!! = %s\n",Global::EQDataReturn_cmd.Data[i]);
                    printf("routineDATA!!!!!!!!!!!!!!!!!!!! = %s\n",routineDATA[i]);
                }

                break;

            default:
                break;
            }

        }

        Global::isActionUpdate = false;

        switch ( Global::RqtType )
        {
        case IdRequest:
            sprintf ( returnval, "EQId*%s#1,%s#2,%s#3,%s#4,%s#5,%s#6,%s#7,%s#8,%s#9,%s#10,%s#11,%s#12,%s*&", Global::EQIdReturn_cmd.IP,
                      routineID[0], routineID[1], routineID[2], routineID[3], routineID[4], routineID[5],
                      routineID[6], routineID[7], routineID[8], routineID[9], routineID[10], routineID[11] );
            break;

        case DataRequest:
            sprintf ( returnval, "EQData*%s#1%s#2%s#3%s#4%s#5%s#6%s#7%s#8%s#9%s#10%s#11%s#12%s*&", Global::EQDataReturn_cmd.IP,
                      routineDATA[0], routineDATA[1], routineDATA[2], routineDATA[3], routineDATA[4], routineDATA[5],
                      routineDATA[6], routineDATA[7], routineDATA[8], routineDATA[9], routineDATA[10], routineDATA[11] );
            break;

        default:
            break;
        }

    }

    for ( int i = 0; i < ReturnMsgBuffer; i++ )
    {
        returnmsg[i] = returnval[i];
    }


    return;
}
