#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <time.h>
#include <cerrno>
#include <unistd.h>
#include "DataCapture.h"
#include "Communication.h"
#include <string.h>

MainState main_state[MAX_CONNECTOR] = {Main_None};
SettingState setting_state[MAX_CONNECTOR] = {Setting_None};
RoutineState routine_State[MAX_CONNECTOR] = {Routine_None};
enum enum_ID {id_None = 0, id_SetCmd = 1, id_WaitData = 2, id_Finish = 3, id_NextCmd = 4};      // ID = id
enum enum_BR {br_None = 0, br_SetCmd = 1, br_WaitData = 2, br_Finish = 3};      // Baudrate = BR = br
enum enum_EQ {eq_None = 0, eq_SetCmd = 1, eq_WaitData = 2, eq_Finish = 3, eq_NextCmd = 4};      // EQData = EQ = eq

bool isNeedGetData[MAX_CONNECTOR] = {false}, isNeedSetCmd[MAX_CONNECTOR] = {false};
char cmd[MAX_CONNECTOR], parameter[MAX_CONNECTOR];
enum_ID ID_state[MAX_CONNECTOR] = {id_None};
enum_BR BR_state[MAX_CONNECTOR] = {br_None};
enum_EQ EQ_state[MAX_CONNECTOR] = {eq_None};

char* recv_data[MAX_CONNECTOR];
bool isCorrectData[MAX_CONNECTOR] = {false};
int cmd_loop_total[MAX_CONNECTOR] = {0}, cmd_loop_current[MAX_CONNECTOR] = {0};
char* serial_data[MAX_CONNECTOR];
int time_out[MAX_CONNECTOR] = {0};

char temp_char[MAX_CONNECTOR][32];
char* temp_data[MAX_CONNECTOR];

int port_type[MAX_CONNECTOR] = { -1};

bool isResetFirmware[MAX_CONNECTOR] = {false};

void ParaInit ( int tid )
{
    main_state[tid] = Main_None;
    setting_state[tid] = Setting_None;
    routine_State[tid] = Routine_None;

    isNeedGetData[tid] = false;
    isNeedSetCmd[tid] = false;
    cmd[tid] = '\0';
    parameter[tid] = '\0';
    ID_state[tid] = id_None;
    BR_state[tid] = br_None;
    EQ_state[tid] = eq_None;

    recv_data[tid] = ( char * ) malloc ( BUFFER_SIZE * sizeof ( char ) );

    isCorrectData[tid] = false;
    cmd_loop_total[tid] = 0;
    cmd_loop_current[tid] = 0;
    serial_data[tid] = ( char * ) malloc ( 1024 * sizeof ( char ) );

    time_out[tid] = 0;

    //for (int i =0; i<32; i++)        temp_char[tid][i] = 0;
    memset ( temp_char[tid], 0, sizeof ( temp_char[tid] ) );
    temp_data[tid] = ( char * ) malloc ( 2048 * sizeof ( char ) );


    port_type[tid] = -1;
    isResetFirmware[tid] = false;
}

void InputData ( int tid )
{
    if ( main_state[tid] != Global::enum_MainState )
    {
//        printf ( "Global::enum_MainState = %d\n", Global::enum_MainState );
//        printf ( "main_state[%d] = %d\n",tid, main_state[tid] );
        main_state[tid] = Global::enum_MainState;
    }

    switch ( main_state[tid] )
    {
    case Main_Setting:

//        printf ( "Global::enum_SettingState = %d\n", Global::enum_SettingState );
//        printf ( "setting_state[%d] = %d\n",tid, setting_state[tid] );
        if ( setting_state[tid] != Global::enum_SettingState )
        {
            setting_state[tid] = Global::enum_SettingState;

            switch ( setting_state[tid] )
            {
            case Setting_EQId:
                printf ( "Setting_EQId ~~~~~~~~~~~~~~~~~\n" );
                Global::isSubDataReady[tid] = DataNotReady;
                ID_state[tid] = id_SetCmd;
                time_out[tid] = 20;
                break;

            case Setting_EQBaudrate:
                Global::isSubDataReady[tid] = DataNotReady;

                if ( true == Global::isIdExist[tid] )   BR_state[tid] = br_SetCmd;
                else                                        BR_state[tid] = br_Finish;
                time_out[tid] = 20;
                break;

            case Setting_EQType:
                port_type[tid] = Global::EQType_cmd.PortType[tid];
                break;

            case Setting_EQCmd:
                break;

            default:
                break;
            }
        }

        break;

    case Main_Routine:

//        printf ( "Global::enum_RoutineState = %d\n", Global::enum_RoutineState );
//        printf ( "routine_State[%d] = %d\n",tid, routine_State[tid] );
        if ( Global::isFirstRoutine_DataCapture[tid] )
        {
//            routine_State[tid] = Global::enum_RoutineState;
//
//            switch ( routine_State[tid] )
//            {
//            case Routine_EQId:
//                printf ( "Routine_EQId ~~~~~~~~~~~~~~~~~\n" );
            ID_state[tid] = id_SetCmd;
            EQ_state[tid] = eq_None;
//                break;
//
//            case Routine_EQData:
////                printf ( "Routine_EQData ~~~~~~~~~~~~~~~~~\n" );
//                cmd_loop_total[tid] = 0;
//                cmd_loop_current[tid] = 0;
//                if ( true == Global::isIdExist[tid] )   EQ_state[tid] = eq_SetCmd;
//                else                                       EQ_state[tid] = eq_NextCmd;
//                memset ( serial_data[tid], 0, 1024 * sizeof ( char ) );
//                break;
//
//            default:
//                printf ( "UBHSBDJDHSJSBSJ ~~~~~~~~~~~~~~~~~\n" );
//                break;
//            }

            time_out[tid] = 20;
            Global::isFirstRoutine_DataCapture[tid] = false;
        }
        break;

    default:
//        printf ( "GlobalGlobalGlobalGlobal\n" );
        break;
    }

    if(true == isResetFirmware[tid])
    {
        isResetFirmware[tid] = false;
        Global::enum_RoutineState = Routine_EQId;
    }
    else
    {
        if ( isNeedGetData[tid] )
        {
            memset ( recv_data[tid], 0, BUFFER_SIZE * sizeof ( char ) );
            recv_data[tid] = ReadMsg ( tid );
//        printf ( "[tid = %d]ReadMsg + 2: %X\n", tid,  * ( recv_data[tid] + 2 ) );

            if ( ( * ( recv_data[tid] + 0 ) == 0x7E ) && ( * ( recv_data[tid] + 1 ) == 0x7E ) )
            {
                if ( * ( recv_data[tid] + 2 ) == 0xBB )
                {
                    isNeedSetCmd[tid] = true;
                }
                else
                {
                    isNeedGetData[tid] = false;
                    isCorrectData[tid] = true;
                }
            }
            else        // if get error data over 20 times, time out trigger.
            {
                printf ( "time out[%d] = %d\n", tid, time_out[tid] );

                if ( 0 >= time_out[tid]-- )
                {
                    isNeedGetData[tid] = false;

                    switch ( main_state[tid] )
                    {
                    case Main_Setting:
                        switch ( setting_state[tid] )
                        {
                        case Setting_EQId:
                            sprintf ( temp_char[tid], "%X", 0xFF );
                            ID_state[tid] = id_Finish;
                            break;

                        case Setting_EQBaudrate:
                            sprintf ( temp_char[tid], "%X", 0xFF );
                            BR_state[tid] = br_Finish;
                            break;

                        case Setting_EQType:
                            break;

                        case Setting_EQCmd:
                            break;

                        default:
                            break;
                        }

                        break;

                    case Main_Routine:
                        switch ( routine_State[tid] )
                        {
                        case Routine_EQId:
                            sprintf ( temp_char[tid], "%X", 0xFF );
                            ID_state[tid] = id_Finish;
                            break;

                        case Routine_EQData:
                            sprintf ( temp_data[tid], "%X", 0xFF );
                            EQ_state[tid] = eq_Finish;
                            break;

                        default:
                            break;
                        }

                        break;

                    default:
                        break;
                    }
                }
                else
                {
                    isNeedSetCmd[tid] = true;
                    switch ( main_state[tid] )
                    {
                    case Main_Setting:
                        switch ( setting_state[tid] )
                        {
                        case Setting_EQId:
                            ID_state[tid] = id_SetCmd;
                            printf ( "time out[%d] = %d - sid_SetCmd\n", tid, (time_out[tid]+1) );
                            break;

                        case Setting_EQBaudrate:
                            BR_state[tid] = br_SetCmd;
                            printf ( "time out[%d] = %d - br_SetCmd\n", tid, (time_out[tid]+1) );
                            break;

                        case Setting_EQType:
                            break;

                        case Setting_EQCmd:
                            break;

                        default:
                            break;
                        }

                        break;

                    case Main_Routine:
                        switch ( routine_State[tid] )
                        {
                        case Routine_EQId:
                            ID_state[tid] = id_SetCmd;
                            printf ( "time out[%d] = %d - id_SetCmd\n", tid, (time_out[tid]+1) );
                            break;

                        case Routine_EQData:
                            cmd_loop_current[tid]--;
                            EQ_state[tid] = eq_SetCmd;
                            printf ( "time out[%d] = %d - eq_SetCmd\n", tid, (time_out[tid]+1) );
                            break;

                        default:
                            break;
                        }

                        break;

                    default:
                        break;
                    }
                }
            }

            //for ( int i = 0; i < BUFFER_SIZE; i++ )     printf ( "data[%d] = %X;\n", i, * ( recv_data[tid] + i ) );

        }
    }

    if(Routine_EQReset == Global::enum_RoutineState)
    {
        isResetFirmware[tid] = true;
    }


}
void ProcessData ( int tid )
{
    switch ( main_state[tid] )
    {
    case Main_Setting:
        switch ( setting_state[tid] )
        {
        case Setting_EQId:

            switch ( ID_state[tid] )
            {
            case id_SetCmd:
//                printf ( "id_SetCmd[tid = %d]: --------\n",tid);
                memset ( temp_char[tid], 0, BUFFER_SIZE * sizeof ( char ) );
                cmd[tid] = 0x32;
                parameter[tid] = 0x00;
                isNeedSetCmd[tid] = true;
                ID_state[tid] = id_WaitData;
                break;

            case id_WaitData:

//                printf ( "id_WaitData[tid = %d]: --------\n",tid);
                if ( isCorrectData[tid] )
                {
//                    printf ( "recv_data: %X\n", * ( recv_data + 2 ) );
                    if ( ( * ( recv_data[tid] + 2 ) == 0xFF ) || ( * ( recv_data[tid] + 2 ) == 0xCC ) )
                    {
 /*                       if( * ( recv_data[tid] + 2 ) == 0xCC ){
                          printf("isCorrectData recv_data[%d] + 2 == 0xCC\n", tid);
                          sprintf ( temp_char[tid], "%X", 0xCC );
                        }
                        else if(* ( recv_data[tid] + 2 ) == 0xFF){
                          sprintf ( temp_char[tid], "%X", 0xFF );
                        }
   */                   sprintf ( temp_char[tid], "%X", 0xFF );
                        Global::isIdExist[tid] = false;
                        ID_state[tid] = id_Finish;
                    }
                    else
                    {
                        if ( ( * ( recv_data[tid] + 2 ) == 0xA2 ) && ( * ( recv_data[tid] + 7 ) == 0xED ) )
                        {
                            char *pch_sn = ( char* ) malloc ( 4 * sizeof ( char ) );
                            memmove ( pch_sn, recv_data[tid] + 3, 4 );
//                            for ( int i = 0; i < 4; i++ )    printf ( "pch_sn[%d] = %X;\n", i, * ( pch_sn + i ) );
                            sprintf ( temp_char[tid], "%X", * ( pch_sn + 3 ) );
                            int pint_sn = atoi ( temp_char[tid] );

                            sprintf ( temp_char[tid], "SN%05d", pint_sn );
                            Global::isIdExist[tid] = true;
                            free ( pch_sn );
                            ID_state[tid] = id_Finish;
                            printf("33333333333333333333333333");

                            Global::isHardwareThere[tid] = true;

                        }
                        else if ( ( * ( recv_data[tid] + 2 ) == 0xBB ))
                        {
                            sprintf ( temp_char[tid], "%s", "BUSY" );
                            Global::isIdExist[tid] = false;
                            ID_state[tid] = id_SetCmd;
                            printf("44444444444444444444444444");
                        }
                        else
                        {
                            printf("temp_char recv_data[%d] = %X" , tid, recv_data[tid]);
                            sprintf ( temp_char[tid], "%s", "" );
                            Global::isIdExist[tid] = false;
                            ID_state[tid] = id_SetCmd;
                            printf("55555555555555555555555555");
                        }
                    }

                    isCorrectData[tid] = false;
                }

                break;

            case id_Finish:

                Global::isDataUpdate[tid] = true;

                if  ( !Global::isProcessUpdate[tid] )
                {
                    Global::temp_EQIdReturn_cmd.PortID[tid] = temp_char[tid];
                    Global::isSubDataReady[tid] = DataReady;
                    ID_state[tid] = id_None;
                }

                Global::isDataUpdate[tid] = false;

                break;

            default:

                break;
            }

            break;

        case Setting_EQBaudrate:

            switch ( BR_state[tid] )
            {
            case br_SetCmd:

                memset ( temp_char[tid], 0, 32 * sizeof ( char ) );
                cmd[tid] = 0x31;
                parameter[tid] = ParaBaudrate ( tid );
                isNeedSetCmd[tid] = true;
                BR_state[tid] = br_WaitData;
                break;

            case br_WaitData:

                if ( isCorrectData[tid] )
                {
                    if ( * ( recv_data[tid] + 2 ) == 0xAA )
                    {

                        sprintf ( temp_char[tid], "%X", * ( recv_data[tid] + 2 ) );
                    }
                    else
                    {
                        sprintf ( temp_char[tid], "%X", 0xDD );
                    }

                    isCorrectData[tid] = false;
                    BR_state[tid] = br_Finish;
                }

                break;

            case br_Finish:
                Global::isDataUpdate[tid] = true;

                if  ( !Global::isProcessUpdate[tid] )
                {
                    if ( strcmp ( temp_char[tid], "AA" ) == 0 )    Global::temp_EQBaudrateReturn_cmd.PortSetting[tid] = 1;
                    else                                        Global::temp_EQBaudrateReturn_cmd.PortSetting[tid] = 0;

                    Global::isSubDataReady[tid] = DataReady;
                    BR_state[tid] = br_None;
                }

                Global::isDataUpdate[tid] = false;

                break;

            default:
                break;
            }

            break;

        case Setting_EQType:
            break;

        case Setting_EQCmd:
            break;

        default:
            break;
        }

        break;

    case Main_Routine:
        if(false == isResetFirmware[tid])
        {
            switch ( ID_state[tid] )
            {
            case id_SetCmd:
//            printf ( "Main_Routine.....id_SetCmd..........\n" );
                memset ( temp_char[tid], 0, BUFFER_SIZE * sizeof ( char ) );
                cmd[tid] = 0x32;
                parameter[tid] = 0x00;
                isNeedSetCmd[tid] = true;
                ID_state[tid] = id_WaitData;
                break;

            case id_WaitData:

//            printf ( "Main_Routine.....id_WaitData..........\n" );
                if ( isCorrectData[tid] )
                {
//                printf ( "recv_data: %X\n",* ( recv_data + 2 ) );
                    if ( ( * ( recv_data[tid] + 2 ) == 0xFF ) || ( * ( recv_data[tid] + 2 ) == 0xCC ) )
                    {
                        if( * ( recv_data[tid] + 2 ) == 0xCC ){
                          printf("id_WriteDat recv_data[%d] + 2 == 0xCC\n", tid);
                        }
                        else{
                          printf("id_WriteDat recv_data[%d] + 2 == 0xFF\n", tid);
                        }
                        printf("77777777777777777777777777\n");
                        sprintf ( temp_char[tid], "%s", "" );
                    }
                    else
                    {
//                    char *pch_sn = ( char* ) malloc ( 4 * sizeof ( char ) );
//                    memmove ( pch_sn, recv_data[tid] + 3, 4 );
////                    for ( int i = 0; i < 4; i++ )    printf ( "pch_sn[%d] = %X;\n", i, * ( pch_sn + i ) );
//                    sprintf ( temp_char[tid], "%X", * ( pch_sn + 3 ) );
//                    int pint_sn = atoi ( temp_char[tid] );
//
//                    sprintf ( temp_char[tid], "SN%05d", pint_sn );
//
//                    free ( pch_sn );


                        if ( ( * ( recv_data[tid] + 2 ) == 0xA2 ) && ( * ( recv_data[tid] + 7 ) == 0xED ) )
                        {
                            char *pch_sn = ( char* ) malloc ( 4 * sizeof ( char ) );
                            memmove ( pch_sn, recv_data[tid] + 3, 4 );
//                            for ( int i = 0; i < 4; i++ )    printf ( "pch_sn[%d] = %X;\n", i, * ( pch_sn + i ) );
                            sprintf ( temp_char[tid], "%X", * ( pch_sn + 3 ) );
                            int pint_sn = atoi ( temp_char[tid] );

                            sprintf ( temp_char[tid], "SN%05d", pint_sn );

                            free ( pch_sn );
                            printf("8888888888888811111111111111\n");
                        }
                        else if( * ( recv_data[tid] + 2 ) == 0xBB ){
                            printf("tid = %d , Message = %s", tid, recv_data[tid]);
                            sprintf ( temp_char[tid], "%s", "BUSY" );
                            printf("888888888888882222222222222222222\n");
                        }
                        else
                        {
                            printf("tid = %d , Message = %s", tid, recv_data[tid]);
                            sprintf ( temp_char[tid], "%s", "" );
                            printf("8888888888888833333333333333333\n");
                        }
                    }

                    isCorrectData[tid] = false;
                    ID_state[tid] = id_Finish;
                }

                break;

            case id_Finish:
//            printf ( "Main_Routine.....id_Finish..........\n" );
                Global::isRoutineIdUpdate[tid] = true;

                if  ( !Global::isProcessEqUpdate[tid] )
                {
//                strcpy (  Global::temp_EQIdReturn_cmd.PortID[tid] , temp_char[tid] );
                    sprintf(  Global::temp_EQIdReturn_cmd.PortID[tid] ,"%s", temp_char[tid] );
                    ID_state[tid] = id_NextCmd;
                }

//            printf("temp_CHAR = %s,\n", temp_char[tid]);
                Global::isRoutineIdUpdate[tid] = false;
                break;

            case id_NextCmd:
//            printf ( "Main_Routine.....id_NextCmd..........\n" );
                // init EQ_state
                cmd_loop_total[tid] = 0;
                cmd_loop_current[tid] = 0;

                if ( true == Global::isIdExist[tid] )   EQ_state[tid] = eq_SetCmd;
                else                                       EQ_state[tid] = eq_NextCmd;

                memset ( serial_data[tid], 0, 1024 * sizeof ( char ) );

                time_out[tid] = 20;
                ID_state[tid] = id_None;
                break;

            default:
                break;
            }

            //Global::SettingProcedure >=0x0F;
            switch ( EQ_state[tid] )
            {
            case eq_SetCmd:
//            printf ( "Main_Routine......eq_SetCmd........\n" );
                cmd[tid] = 0x33;
                parameter[tid] = ParaEqData ( tid );
                isNeedSetCmd[tid] = true;
                memset ( temp_data[tid], 0, 2048 * sizeof ( char ) );
                EQ_state[tid] = eq_WaitData;
                break;

            case eq_WaitData:

//            printf ( "Main_Routine......eq_WaitData........\n" );
                if ( isCorrectData[tid] )
                {
                    //for ( int i = 0; i < 32; i++ )      printf ( "recv_data[tid = %d][%d]: %02X\n",tid, i, * ( recv_data[tid] + i ) );

                    memmove ( serial_data[tid] + ( cmd_loop_current[tid] - 1 ) * 28, recv_data[tid] + 2, 28 );

                    if ( cmd_loop_current[tid] >= cmd_loop_total[tid] )
                    {

                        ParseData ( tid );

                        EQ_state[tid] = eq_Finish;
//                    printf ( "EQ_state[%d] = %d \n",tid, EQ_state[tid] );
                    }
                    else
                    {
                        isCorrectData[tid] = false;
                        EQ_state[tid] = eq_SetCmd;
                    }
                }

                break;

            case eq_Finish:
//            printf ( "eq_Finish........[tid = %d]\n", tid );
                Global::isRoutineEqUpdate[tid] = true;

                if  ( !Global::isProcessEqUpdate[tid] )
                {
//                printf ( "temp_data--- = %s\n", temp_data[tid] );
//                printf ( "temp_EQDataReturn_cmd--- = %s\n", Global::temp_EQDataReturn_cmd.Data[tid] );

//                   memmove( Global::temp_EQDataReturn_cmd.Data[tid] ,  temp_data[tid], 2048*sizeof(char));
                    sprintf ( Global::temp_EQDataReturn_cmd.Data[tid] , "%s", temp_data[tid] );

//                printf ( "=============================\n\n[tid= %d]temp_EQDataReturn_cmd--- = %s\n\n================================\n", tid, Global::temp_EQDataReturn_cmd.Data[tid] );
//            memmove(   Global::temp_EQDataReturn_cmd.Data[tid] ,temp_data[tid], 2048*sizeof(char)) ;

                    EQ_state[tid] = eq_NextCmd;
                }

                Global::isRoutineEqUpdate[tid] = false;

                break;

            case eq_NextCmd:
                // init ID_state
                ID_state[tid] = id_SetCmd;

                time_out[tid] = 20;
                EQ_state[tid] = eq_None;
                break;

            default:
                break;
            }
        }
        else
        {
//            printf ( "Main_Routine......EQReset........\n" );
            cmd[tid] = 0x35;
            parameter[tid] = 0x03;
            isNeedSetCmd[tid] = true;
            usleep ( 2000000 );
        }
        break;

    default:
        break;
    }
}
void OutputData ( int tid )
{
    if ( isNeedSetCmd[tid] )
    {
        if  ( true == SendMsg ( tid, cmd[tid], parameter[tid] ) )
        {
//            time_out[tid] = 20;
            isNeedGetData[tid] = true;
            isCorrectData[tid] = false;
            isNeedSetCmd[tid] = false;
        }
        else
        {
            printf ( "send error[tid = %d]: --------\n", tid );
        }
    }
}

void *DataCapture ( void *threadid )
{
    int  tid = ( int ) threadid;
    printf ( "Thread ID : %d\n", tid );

//    while ( !Global::isExitThread )
//    {
    ParaInit ( tid );

//        if ( !Global::isCommunicationOpened[tid] )
//            Global::isCommunicationOpened[tid] = TerminalCommunicationOpen ( tid );

    //SendMsg ( 0x31, 0x0D );
    //SendMsg ( 0x32, 0x00 );
    //SendMsg ( 0x33, 0x00 );   //KCM
    //SendMsg ( 0x33, 0x01 );   //SDC15 0x01,0x11,0x21,0x31,0x41.
    //SendMsg ( 0x33, 0x02 );   //HUSKY 0x02,0x12,0x22,0x32,0x42,0x52,0x62,0x72,0x82.
    //SendMsg ( 0x33, 0x06 );   //HUSKY 0x06,0x16,0x26,0x36,0x46,0x56,0x66,0x76,0x86.

//    for ( int i = 0; i < 12; i++ )
//    {
//        temp_data[i] = ( char* ) malloc ( 2048 * sizeof ( char ) );
//        memset ( temp_data[i], 0, 2048 * sizeof ( char ) );
//    }
//        reset_time[tid] = 60;

//        while ( ( !Global::isExitThread ) && ( 0 <= reset_time[tid]-- ) )
    while ( !Global::isExitThread )
    {
//        printf("InputData\n");
        printf ( "[Thread Id = %d]: InputData\n", tid );
        InputData ( tid );
//        printf("ProcessData\n");

        printf ( "[Thread Id = %d]: ProcessData\n", tid );
        ProcessData ( tid );
//        printf("OutputData\n");
        printf ( "[Thread Id = %d]: OutputData\n", tid );
        OutputData ( tid );

        usleep ( 1000000 );

//        Global::EQBaudrateReturn_cmd.PortSetting[11]++;
    }

//        TerminalCommunicationClose ( tid );
    Global::isCommunicationOpened[tid] = false;

    free ( recv_data[tid] );
    free ( serial_data[tid] );
    free ( temp_data[tid] );


    sprintf (  Global::EQIdReturn_cmd.PortID[tid] , "%X", 0xFF );
    sprintf (  Global::temp_EQIdReturn_cmd.PortID[tid] , "%X", 0xFF );

    sprintf (  Global::EQDataReturn_cmd.Data[tid] , "%X", 0xFF );
    sprintf (  Global::temp_EQDataReturn_cmd.Data[tid] , "%X", 0xFF );

//        printf ( "RESETing RESETing >>>>> [%d]\n", tid );
//    }

    printf ( "pthread_exit[%d]\n", tid );
    //pthread_exit(NULL);
    return NULL;
}

//////////////////////////
// sub-function
//////////////////////////

char ParaBaudrate ( int tid )
{
    char ret = 0x00;

    switch ( Global::EQBaudrate_cmd.baurate[tid] )
    {
    case 300:
        ret += 0x00;
        break;

    case 1200:
        ret += 0x01;
        break;

    case 2400:
        ret += 0x02;
        break;

    case 4800:
        ret += 0x03;
        break;

    case 9600:
        ret += 0x04;
        break;

    case 19200:
        ret += 0x05;
        break;

    case 115200:
        ret += 0x06;
        break;

    default:
        break;
    }


    char* data = ( char* ) malloc ( 1024 * sizeof ( char ) );
    memcpy ( data, Global::EQBaudrate_cmd.dataformat[tid], strlen ( Global::EQBaudrate_cmd.dataformat[tid] ) );
    char* pch = strtok ( data, "," );   // 8,N,1

    if ( strcmp ( pch, "8" ) == 0 )     ret += 0x08;
    else                                 ret += 0x00;

    pch = strtok ( NULL, "," );     //N,1

    if ( strcmp ( pch, "O" ) == 0 )          ret += 0x20;
    else if ( strcmp ( pch, "E" ) == 0 )    ret += 0x40;
    else                                      ret += 0x00;

    pch = strtok ( NULL, "," );     //1

    if ( strcmp ( pch, "2" ) == 0 )     ret += 0x80;
    else                                ret += 0x00;

    free ( data );
    //printf ( "ret = %X\n", ret );
    return ret;
}

char ParaEqData ( int tid )
{
    char ret = 0x00;
    char seq[16] = {0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0};

    switch ( port_type[tid] )
    {
    case KCM:
        cmd_loop_total[tid] = 1;
        ret = 0x00;
        printf ( "KCM\n" );
        break;

    case SDC15:
        cmd_loop_total[tid] = 5;
        ret = 0x01;
        printf ( "SDC15\n" );
        break;

    case HUSKY:
        cmd_loop_total[tid] = 9;
        ret = 0x02;
        printf ( "HUSKY\n" );
        break;

    default:
        cmd_loop_total[tid] = 0;
        printf ( "ERROR MACHINE\n" );
        break;
    }

    ret += seq[cmd_loop_current[tid]];
    cmd_loop_current[tid]++;
//    printf ( "tid = %d\n", tid );
//    printf ( "port_type[%d] = %d\n",tid, port_type[tid] );
//    printf ( "Global::EQType_cmd.PortType[tid] = %d\n", Global::EQType_cmd.PortType[tid] );
//    printf ( "cmd_loop_total[%d] = %d\n",tid, cmd_loop_total[tid] );
//    printf ( "\n#############################\n-------  EqDataCmd = %X\n !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", ret );
    return ret;
}

void ParseData ( int tid )
{
    //for ( int i = 0; i < 127; i++ )      printf ( "serial_data[%d]: %02X     serial_data[%d]: %02X\n", 2 * i, * ( serial_data + 2 * i ), 2 * i + 1, * ( serial_data + 2 * i + 1 ) );

    int cmd_num = 0, cmd_size = 0;
    char *split_data = ( char* ) malloc ( 2 * BUFFER_SIZE * sizeof ( char ) );
    //char split_data[2 * BUFFER_SIZE] ={0};

    switch ( port_type[tid] )
    {
    case KCM:
        cmd_num = 1;
        cmd_size = 7;
        break;

    case SDC15:
        cmd_num = 12;
        cmd_size = 9;
        break;

    case HUSKY:
        cmd_num = 12;
        cmd_size = 18;
        break;

    default:
        break;
    }

    //sprintf(temp_data[tid],"a");
    if ( KCM == port_type[tid] )
    {
        for ( int i = 0; i < cmd_num; i++ )
        {
            MainState state = Global::enum_MainState;
            memset ( split_data, 0, 2 * BUFFER_SIZE * sizeof ( char ) );
            sprintf ( split_data, "@%02X", * ( serial_data[tid] + ( cmd_size  ) * i ) );

            for ( int j = 0; j < cmd_size - 1; j++ )
            {
                char temp[10] = {'\0'};
                sprintf ( temp, "-%02X", * ( serial_data[tid] + ( ( cmd_size ) * i ) + 1 + j ) );
                strcat ( split_data, temp );
            }

            strcat ( temp_data[tid], split_data ) ;
            Global::enum_MainState = state;
        }
    }
    else
    {
        for ( int i = 0; i < cmd_num; i++ )
        {
//            MainState state = Global::enum_MainState;
            memset ( split_data, 0, 2 * BUFFER_SIZE * sizeof ( char ) );
            sprintf ( split_data, "@%02X", * ( serial_data[tid] + ( cmd_size + 1 ) * i + 1 ) );

            for ( int j = 0; j < cmd_size - 1; j++ )
            {
                char temp[10] = {'\0'};
                sprintf ( temp, "-%02X", * ( serial_data[tid] + ( ( cmd_size + 1 ) * i + 1 ) + 1 + j ) );
                strcat ( split_data, temp );
            }

//
//        printf ( "Global::enum_MainState = %d\n", Global::enum_MainState );
//            printf ( "split_data(%d): %s\n", i,  split_data );
//
            strcat ( temp_data[tid], split_data ) ;
//        printf ( "Global::enum_MainState = %d\n", Global::enum_MainState );
//            Global::enum_MainState = state;
        }
    }

    //
    free ( split_data );
//    printf ( "temp_data[%d]: %s\n", tid, temp_data[tid] );
}



