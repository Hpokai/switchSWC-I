#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>

#include "Global.h"
#include "Definition.h"

using namespace std;

char* ip_str = ( char* ) malloc ( BUFFER_SIZE * sizeof ( char ) );
int interpreter ( char *cmd ) //, size_t cmdsize )
{
    int ret = 0;

    printf ( "cmd = %s\n", cmd );

    char* pch = strtok ( cmd, " *@#&" );     // EQXX
    printf ( "pch = %s\n", pch );

    if ( strcmp ( pch, "EQId" ) == 0 )
    {
        Global::RqtType = IdRequest;
        Global::SettingProcedure |= Setting_EQId;

        for ( int i = 0; i < 1; i++ ) Global::isSubDataReady[i] = DataNotReady;

        switch ( Global::enum_MainState )
        {
        case Main_Setting:
            Global::enum_SettingState = Setting_EQId;
        printf ( "Global::enum_SettingState = %d\n", Global::enum_SettingState );
            break;

        case Main_Routine:
            Global::enum_RoutineState = Routine_EQId;
            break;

        default:
            break;
        }

        pch = strtok ( NULL, " *@#&" );
        strcpy (  ip_str , pch );
       strcpy(Global::EQId_cmd.IP, ip_str);
        printf ( "Global::EQId_cmd.IP = %s\n", Global::EQId_cmd.IP );

        for ( int i = 0; i < MAX_CONNECTOR; i++ )
        {
            pch = strtok ( NULL, " *@#&" );
            Global::EQId_cmd.ID[i] = pch;
        }

        for ( int i = 0; i < MAX_CONNECTOR; i++ )
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

        for ( int i = 0; i < 1; i++ ) Global::isSubDataReady[i] = DataNotReady;

        switch ( Global::enum_MainState )
        {
        case Main_Setting:
            Global::enum_SettingState = Setting_EQBaudrate;
            break;

        case Main_Routine:
            Global::enum_MainState = Main_Setting;
            Global::enum_SettingState = Setting_EQBaudrate;
            Global::SettingProcedure = 0;
            Global::SettingProcedure |= Setting_EQId;
            Global::SettingProcedure |= Setting_EQBaudrate;
            break;

        default:
            break;
        }

        char* temp_cmd[MAX_CONNECTOR] = {'\0'};

        for ( int i = 0; i < MAX_CONNECTOR; i++ )
        {
            pch = strtok ( NULL, " *@#&" );
            temp_cmd[i] = pch;
            //printf ( "temp_cmd[%d] = %s\n", i,temp_cmd[i] );
        }

        for ( int i = 0; i < MAX_CONNECTOR; i++ )
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

        switch ( Global::enum_MainState )
        {
        case Main_Setting:
            Global::enum_SettingState = Setting_EQType;
            break;

        case Main_Routine:
            Global::enum_MainState = Main_Setting;
            Global::enum_SettingState = Setting_EQType;
            break;

        default:
            break;
        }


        char* temp_cmd[3] = {'\0'};

        for ( int i = 0; i < 3; i++ )
        {
            pch = strtok ( NULL, " *@#&" );;
            temp_cmd[i] = pch;
//            printf ( "temp_cmd[%d] = %s\n", i, temp_cmd[i] );
        }

        char* temp_str[3] = {'\0'};

        for ( int i = 0; i < 3; i++ )
        {
            Global::EQType_cmd.CmdType[i] = strtok ( temp_cmd[i], "$" );
            Global::EQType_cmd.CmdNum[i] = atoi ( strtok ( NULL, "N:" ) );
            temp_str[i] = strtok ( NULL, "&" );

//            printf ( "CmdType[%d] = %s\n", i, Global::EQType_cmd.CmdType[i] );
//            printf ( "CmdNum [%d] = %d\n", i, Global::EQType_cmd.CmdNum[i] );
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
                else
                {
                    Global::EQType_cmd.PortType[index] = -1;
                }

                pch = strtok ( NULL, "," );

                if  ( pch == NULL )   j = Global::EQType_cmd.CmdNum[i];
            }
        }

        ret = 1;
    }
    else if ( strcmp ( pch, "EQCmd" ) == 0 )
    {
        Global::RqtType = None;
        Global::SettingProcedure |= Setting_EQCmd;

        switch ( Global::enum_MainState )
        {
        case Main_Setting:
            Global::enum_SettingState = Setting_EQCmd;
            break;

        case Main_Routine:
            Global::enum_MainState = Main_Setting;
            Global::enum_SettingState = Setting_EQCmd;
            break;

        default:
            break;
        }

        char* temp_cmd[128] = {'\0'};

        for ( int i = 0; i < 128; i++ )     temp_cmd[i] = ( char* ) malloc ( 32 * sizeof ( char ) );

        int index = 0;

        while ( pch != NULL )
        {
            pch = strtok ( NULL, "*@#&" );

            if ( pch != NULL )  strcpy ( temp_cmd[index], pch );

//            printf ( "temp_cmd[%d] = %s\n", index, temp_cmd[index] );
            index++;
        }

        int KCM_index = 0, Husky_index = 0, SDC15_index = 0;

        for ( int i = 0; i < ( index - 1 ); i++ )
        {
            if ( NULL != strstr ( temp_cmd[i], "KCM" ) )
            {
                pch = strtok ( temp_cmd[i], ":" );
                Global::EQCmd_cmd.KCMCmd[KCM_index] = strtok ( NULL, ":" );
//                printf ( "KCMCmd[%d] = %s\n", KCM_index, Global::EQCmd_cmd.KCMCmd[KCM_index]  );
                KCM_index++;
            }
            else if ( NULL != strstr ( temp_cmd[i], "Husky" ) )
            {
                pch = strtok ( temp_cmd[i], ":" );
                Global::EQCmd_cmd.HUSKYCmd[Husky_index] = strtok ( NULL, ":" );
//                printf ( "HUSKYCmd[%d] = %s\n", Husky_index, Global::EQCmd_cmd.HUSKYCmd[Husky_index]  );
                Husky_index++;
            }
            else if ( NULL != strstr ( temp_cmd[i], "SDC15" ) )
            {
                pch = strtok ( temp_cmd[i], ":" );
                Global::EQCmd_cmd.SDC15Cmd[SDC15_index] = strtok ( NULL, ":" );
//                printf ( "SDC15Cmd[%d] = %s\n", SDC15_index, Global::EQCmd_cmd.SDC15Cmd[SDC15_index]  );
                SDC15_index++;
            }
            else{}
        }

        ret = 1;
    }
    else if ( strcmp ( pch, "EQData" ) == 0 )
    {
        Global::RqtType = DataRequest;

        for ( int i = 0; i < 1; i++ ) Global::isRoutineDataReady[i] = DataNotReady;
        switch ( Global::enum_MainState )
        {
        case Main_Setting:
            for ( int i = 0; i < MAX_CONNECTOR; i++ ) Global::isFirstRoutine_DataCapture[i] = true;
            Global::enum_MainState = Main_Routine;
            Global::enum_RoutineState = Routine_EQData;
            break;

        case Main_Routine:
            Global::enum_RoutineState = Routine_EQData;
            break;

        default:
            break;
        }

        pch = strtok ( NULL, " *@#&" );
        Global::EQData_cmd.CmdNum = atoi ( pch );
//        printf ( "CmdNum = %d\n", Global::EQData_cmd.CmdNum );

        ret = 1;
    }
    else
    {
        Global::RqtType = None;
    }

    return ret;
}
