#include <stdio.h>
#include "Global.h"
#include "Definition.h"
#include <string>
#include <stdlib.h>
#include <iostream>

using namespace std;

//static int lalala =  234;

int interpreter ( char *cmd, size_t cmdsize )
{
    int ret = 0;

    const char *char_star = "*";
    const char *char_hash = "#";
    const char *char_comma = ",";
    const char *char_at = "@";
    const char *char_at_hash = "@#";
    const char *char_dollar = "$";
    const char *char_ncolon = "N:";
    const char *char_colon_hash = ":#";

    printf ( "cmd_size = %d\n", cmdsize );
    printf ( "cmd = %s\n", cmd );

    char* pch = strtok ( cmd, " *@#&" );     // EQXX

    if ( strcmp ( pch, "EQId" ) == 0 )
    {
        Global::RqtType = IdRequest;
        Global::SettingProcedure |= Setting_EQId;

        pch = strtok ( NULL, " *@#&" );
        Global::EQId_cmd.IP = pch;
        //printf ( "Global::EQId_cmd.IP = %s\n", Global::EQId_cmd.IP );

        for ( int i = 0; i < 12; i++ )
        {
            pch = strtok ( NULL, " *@#&" );
            Global::EQId_cmd.ID[i] = pch;
        }

        for ( int i = 0; i < 12; i++ )
        {
            Global::EQId_cmd.ID[i] = strtok ( Global::EQId_cmd.ID[i], "," );
            Global::EQId_cmd.ID[i] = strtok ( NULL, "," );
            //printf ( "Global::EQId_cmd.ID[%d] = %s\n", i , Global::EQId_cmd.ID[i] );
        }

        ret = 1;
    }
    else if ( strcmp ( pch, "EQBaudrate" ) == 0 )
    {
        Global::RqtType = BaudrateRequest;
        Global::SettingProcedure |= Setting_EQBaudrate;

        char* temp_cmd[12] = {'\0'};

        for ( int i = 0; i < 12; i++ )
        {
            pch = strtok ( NULL, " *@#&" );
            temp_cmd[i] = pch;
            //printf ( "temp_cmd[%d] = %s\n", i,temp_cmd[i] );
        }

        for ( int i = 0; i < 12; i++ )
        {
            Global::EQBaudrate_cmd.port[i] = atoi ( strtok ( temp_cmd[i], " ," ) );
            Global::EQBaudrate_cmd.baurate[i] = atoi ( strtok ( NULL, " ," ) );
            Global::EQBaudrate_cmd.dataformat[i] = strtok ( NULL, "&" );

//            printf ( "port      [%d] = %d\n", i,Global::EQBaudrate_cmd.port[i] );
//            printf ( "baurate   [%d] = %d\n", i,Global::EQBaudrate_cmd.baurate[i] );
//            printf ( "dataformat[%d] = %s\n", i,Global::EQBaudrate_cmd.dataformat[i] );
        }

        ret = 1;
    }
    else if ( strcmp ( pch, "EQType" ) == 0 )
    {
        Global::RqtType = None;
        Global::SettingProcedure |= Setting_EQType;
        char* temp_cmd[3] = {'\0'};

        for ( int i = 0; i < 3; i++ )
        {
            pch = strtok ( NULL, " *@#&" );;
            temp_cmd[i] = pch;
            printf ( "temp_cmd[%d] = %s\n", i, temp_cmd[i] );
        }

        char* temp_str[3] = {'\0'};

        for ( int i = 0; i < 3; i++ )
        {
            Global::EQType_cmd.CmdType[i] = strtok ( temp_cmd[i], "$" );
            Global::EQType_cmd.CmdNum[i] = atoi ( strtok ( NULL, "N:" ) );
            temp_str[i] = strtok ( NULL, "&" );

            printf ( "CmdType[%d] = %s\n", i, Global::EQType_cmd.CmdType[i] );
            printf ( "CmdNum [%d] = %d\n", i, Global::EQType_cmd.CmdNum[i] );
        }


        for ( int i = 0; i < 3; i++ )
        {
            pch = strtok ( temp_str[i], "," );

            for ( int j = 0; j < Global::EQType_cmd.CmdNum[i] ; j++ )
            {
                int index = atoi ( pch ) - 1;

                if ( strcmp ( Global::EQType_cmd.CmdType[i] , "KCM" ) == 0 )
                {
                    Global::EQType_cmd.PortType[index] = KCM;
                }
                else if ( strcmp ( Global::EQType_cmd.CmdType[i] , "Husky" ) == 0 )
                {
                    Global::EQType_cmd.PortType[index] = HUSKY;
                }
                else if ( strcmp ( Global::EQType_cmd.CmdType[i] , "SDC15" ) == 0 )
                {
                    Global::EQType_cmd.PortType[index] = SDC15;
                }
                else {}

                pch = strtok ( NULL, "," );
                if  (pch == NULL)   j = Global::EQType_cmd.CmdNum[i];
            }
        }

        ret = 1;
    }
    else if ( strncmp ( cmd, "EQCmd", strlen ( "EQCmd" ) ) == 0 )
    {
        Global::RqtType = None;
        Global::SettingProcedure |= Setting_EQCmd;
        char* temp_cmd[128] = {'\0'};

        int index = 0;

        while ( pch != NULL )
        {
            pch = strtok ( NULL, "*@#&" );
            temp_cmd[index] = pch;
            printf ( "temp_cmd[%d] = %s\n", index, temp_cmd[index] );
            index++;
        }

        int KCM_index = 0, Husky_index = 0, SDC15_index = 0;

        for ( int i = 0; i < index; i++ )
        {
            if ( NULL != strstr ( temp_cmd[i], "KCM" ) )
            {
                pch = strtok ( temp_cmd[i], ":" );
                Global::EQCmd_cmd.KCMCmd[KCM_index] = strtok ( NULL, ":" );
                printf ( "KCMCmd[%d] = %s\n", KCM_index, Global::EQCmd_cmd.KCMCmd[KCM_index]  );
                KCM_index++;
            }
            else if ( NULL != strstr ( temp_cmd[i], "Husky" ) )
            {
                pch = strtok ( temp_cmd[i], ":" );
                Global::EQCmd_cmd.HUSKYCmd[Husky_index] = strtok ( NULL, ":" );
                printf ( "HUSKYCmd[%d] = %s\n", Husky_index, Global::EQCmd_cmd.HUSKYCmd[Husky_index]  );
                Husky_index++;
            }
            else if ( NULL != strstr ( temp_cmd[i], "SDC15" ) )
            {
                pch = strtok ( temp_cmd[i], ":" );
                Global::EQCmd_cmd.SDC15Cmd[SDC15_index] = strtok ( NULL, ":" );
                printf ( "SDC15Cmd[%d] = %s\n", SDC15_index, Global::EQCmd_cmd.SDC15Cmd[SDC15_index]  );
                SDC15_index++;
            }
            else    {}
        }

        ret = 1;
    }
    else if ( strncmp ( cmd, "EQData", strlen ( "EQData" ) ) == 0 )
    {
        Global::RqtType = DataRequest;
        pch = strtok ( NULL, " *@#&" );
        Global::EQData_cmd.CmdNum = atoi ( pch );
//        printf ( "CmdNum = %d\n", Global::EQData_cmd.CmdNum );

        ret = 1;
    }
    else
    {
        Global::RqtType = None;
    }

    printf ( "ddddddddddddddddddddddd\n" );

    return ret;
}
