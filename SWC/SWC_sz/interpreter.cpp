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
    bool isMainStateMatched = false;
//    printf ( "cmd = %s\n", cmd );
    char* pch = strtok ( cmd, " *@#&" );     // EQXX
//    printf ( "pch = %s\n", pch );
 //   printf ( "aaaaaaaaaaaaaaaaaa");
    if ( strcmp ( pch, "EQSId" ) == 0 )     // when server setting
    {
 //       printf ( "bbbbbbbbbbbbbbbbbbbbbbbbb");
        if((Global::enum_SettingState != Setting_EQId) || (!(Global::isEQSID)))
        {
            for(int ii=0;ii<MAX_CONNECTOR;ii++){
                Global::HardwareThereCount[ii] = DataGotten;
            }
            Global::RecvDataTimeStep = 0;
            Global::isEQSID = true;
   //         printf ( "dddddddddddddddddddddddd");
            for ( int i = 0; i < 1; i++ ) Global::isSubDataReady[i] = DataNotReady;

            switch ( Global::enum_MainState )
            {
            case Main_Setting:
                Global::enum_SettingState = Setting_EQId;
                break;

            case Main_Routine:
                Global::enum_MainState = Main_Setting;
                Global::enum_RoutineState = Routine_None;
                Global::enum_SettingState = Setting_EQId;
                Global::SettingProcedure = 0;
                break;

            default:
                break;
            }
            Global::RqtType = SIdRequest;
            Global::SettingProcedure |= Setting_EQId;

            pch = strtok ( NULL, " *@#&" );
            strcpy (  ip_str , pch );
            strcpy(Global::EQId_cmd.IP, ip_str);
//        printf ( "Global::EQId_cmd.IP = %s\n", Global::EQId_cmd.IP );

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
    }
    else if ( strcmp ( pch, "EQBaudrate" ) == 0 )
    {printf ( "cccccccccccccccccccccccc");
        printf("EQBaudrate\n");
        Global::isEQSID = false;
        if(Global::enum_SettingState != Setting_EQBaudrate)
        {
            printf("EQBaudrate  EQBaudrate\n");
            for ( int i = 0; i < 1; i++ ) Global::isSubDataReady[i] = DataNotReady;
            switch ( Global::enum_MainState )
            {
            case Main_Setting:
                Global::enum_SettingState = Setting_EQBaudrate;
                isMainStateMatched = true;
                break;

            case Main_Routine:

                break;

            default:
                break;
            }
            if(isMainStateMatched)
            {
                Global::RqtType = BaudrateRequest;
                Global::SettingProcedure |= Setting_EQBaudrate;

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
        }
    }
    else if ( strcmp ( pch, "EQType" ) == 0 )
    {
        if(Global::enum_SettingState != Setting_EQType)
        {
            switch ( Global::enum_MainState )
            {
            case Main_Setting:
                Global::enum_SettingState = Setting_EQType;
                isMainStateMatched = true;
                break;

            case Main_Routine:
                break;

            default:
                break;
            }
            if(isMainStateMatched)
            {
                Global::RqtType = None;
                Global::SettingProcedure |= Setting_EQType;

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
                    if ( strcmp ( pch , "0" ) == 0 )
                    {
//                printf ( "pch [%d] = %s\n", i, pch );
                        pch = strtok ( NULL, "," );
                    }
                    else
                    {
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
                                Global::EQType_cmd.PortType[index] = NoEQ;
                            }

                            pch = strtok ( NULL, "," );

                            if  ( pch == NULL )   j = Global::EQType_cmd.CmdNum[i];
                        }
                    }
                }

//        for ( int i = 0; i < 12; i++ )            printf ( "PortType [%d] = %d\n", i, Global::EQType_cmd.PortType[i] );
                ret = 1;
            }
        }
    }
    else if ( strcmp ( pch, "EQCmd" ) == 0 )
    {
        if(Global::enum_SettingState != Setting_EQCmd)
        {
            switch ( Global::enum_MainState )
            {
            case Main_Setting:
                Global::enum_SettingState = Setting_EQCmd;
                isMainStateMatched = true;
                break;

            case Main_Routine:
                break;

            default:
                break;
            }

            if(isMainStateMatched)
            {
                Global::RqtType = None;
                Global::SettingProcedure |= Setting_EQCmd;

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
                    else {}
                }

                ret = 1;
            }
        }
    }
    else if ( strcmp ( pch, "EQId" ) == 0 )
    {
        if(Global::enum_RoutineState != Routine_EQId)
        {
            for ( int i = 0; i < 1; i++ ) Global::isSubDataReady[i] = DataNotReady;

            switch ( Global::enum_MainState )
            {
            case Main_Setting:
                for ( int i = 0; i < MAX_CONNECTOR; i++ ) Global::isFirstRoutine_DataCapture[i] = true;
                Global::enum_MainState = Main_Routine;
                Global::enum_SettingState = Setting_None;
                Global::enum_RoutineState = Routine_EQId;
                break;

            case Main_Routine:
                Global::enum_RoutineState = Routine_EQId;
                break;

            default:
                break;
            }
            Global::RqtType = IdRequest;

            pch = strtok ( NULL, " *@#&" );
            strcpy (  ip_str , pch );
            strcpy(Global::EQId_cmd.IP, ip_str);
//        printf ( "Global::EQId_cmd.IP = %s\n", Global::EQId_cmd.IP );

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
    }
    else if ( strcmp ( pch, "EQData" ) == 0 )
    {
        if(Global::enum_RoutineState != Routine_EQData)
        {
            for ( int i = 0; i < 1; i++ ) Global::isRoutineDataReady[i] = DataNotReady;
            switch ( Global::enum_MainState )
            {
            case Main_Setting:
                break;

            case Main_Routine:
                Global::enum_RoutineState = Routine_EQData;
                isMainStateMatched = true;
                break;

            default:
                break;
            }
            if(isMainStateMatched)
            {
                Global::RqtType = DataRequest;

                pch = strtok ( NULL, " *@#&" );
                Global::EQData_cmd.CmdNum = atoi ( pch );
//        printf ( "CmdNum = %d\n", Global::EQData_cmd.CmdNum );

                ret = 1;
            }
        }
    }
    else if ( strcmp ( pch, "EQReset" ) == 0 )
    {
        if(Global::enum_RoutineState != Routine_EQReset)
        {
            printf ( "EQReset !!! @@@ ### ***\n");
            switch ( Global::enum_MainState )
            {
            case Main_Setting:
                break;

            case Main_Routine:
                Global::enum_RoutineState = Routine_EQReset;
                break;

            default:
                break;
            }

            ret = 1;
        }
    }
    else
    {
        Global::RqtType = None;
    }

    return ret;
}
