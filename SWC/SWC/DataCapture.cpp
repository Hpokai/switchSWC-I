#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <cerrno>
#include <unistd.h>
#include "DataCapture.h"
#include "Communication.h"

MainState main_state = Main_None;
SettingState setting_state = Setting_None;
RoutineState routine_State = Routine_None;
enum enum_ID {id_None = 0, id_SetCmd = 1, id_WaitData = 2, id_Finish = 3, id_NextCmd = 4};      // ID = id
enum enum_BR {br_None = 0, br_SetCmd = 1, br_WaitData = 2, br_Finish = 3};      // Baudrate = BR = br
enum enum_EQ {eq_None = 0, eq_SetCmd = 1, eq_WaitData = 2, eq_Finish = 3, eq_NextCmd = 4};      // EQData = EQ = eq

bool isNeedGetData = false, isNeedSetCmd = false;
char cmd, parameter;
enum_ID ID_state = id_None;
enum_BR BR_state = br_None;
enum_EQ EQ_state = eq_None;

char* recv_data = ( char * ) malloc ( BUFFER_SIZE * sizeof ( char ) );
bool isCorrectData = false;
int cmd_loop_total = 0, cmd_loop_current = 0;
char* serial_data = ( char* ) malloc ( 256 * sizeof ( char ) );
int time_out = 0;

char temp_char[32] = {'\0'};
char temp_data[1024] = {'\0'};

int port_type = -1;

void InputData ( int tid )
{
    if ( main_state != Global::enum_MainState )
    {
//        printf ( "Global::enum_MainState = %d\n", Global::enum_MainState );
//        printf ( "main_state = %d\n", main_state );
        main_state = Global::enum_MainState;
    }

    switch ( main_state )
    {
    case Main_Setting:
//        printf ( "Global::enum_SettingState = %d\n", Global::enum_SettingState );
//        printf ( "setting_state = %d\n", setting_state );
        if ( setting_state != Global::enum_SettingState )
        {
            setting_state = Global::enum_SettingState;

            switch ( setting_state )
            {
            case Setting_EQId:
//                printf ( "Setting_EQId ~~~~~~~~~~~~~~~~~\n" );
                Global::isSubDataReady[tid] = DataNotReady;
                ID_state = id_SetCmd;
                break;

            case Setting_EQBaudrate:
                Global::isSubDataReady[tid] = DataNotReady;

                if ( true == Global::isIdExist[tid] )   BR_state = br_SetCmd;
                else                                        BR_state = br_Finish;

                break;

            case Setting_EQType:
                port_type = Global::EQType_cmd.PortType[tid];
                break;

            case Setting_EQCmd:
                break;

            default:
                break;
            }
        }

        break;

    case Main_Routine:
        if ( Global::isFirstRoutine_DataCapture[tid] )
        {
            routine_State = Global::enum_RoutineState;

            switch ( routine_State )
            {
            case Routine_EQId:
                break;

            case Routine_EQData:
                cmd_loop_total = 0;
                cmd_loop_current = 0;
                EQ_state = eq_SetCmd;
                memset ( serial_data, 0, 256 * sizeof ( char ) );
                break;

            default:
                break;
            }

            Global::isFirstRoutine_DataCapture[tid] = false;
        }

        break;

    default:
        printf ( "GlobalGlobalGlobalGlobal\n" );
        break;
    }

    if ( isNeedGetData )
    {
        recv_data = ReadMsg();
        printf ( "ReadMsg + 2: %X\n", * ( recv_data + 2 ) );

        if ( ( * ( recv_data + 0 ) == 0x7E ) && ( * ( recv_data + 1 ) == 0x7E ) )
        {
            if ( * ( recv_data + 2 ) == 0xBB )
            {
                isNeedSetCmd = true;
            }
            else
            {
                isNeedGetData = false;
                isCorrectData = true;
            }
        }
        else        // if get error data over 20 times, time out trigger.
        {
            printf ( "time out = %d\n", time_out );

            if ( 0 >= time_out-- )
            {
                isNeedGetData = false;
                sprintf ( temp_char, "%X", 0xFF );

                switch ( main_state )
                {
                case Main_Setting:
                    switch ( setting_state )
                    {
                    case Setting_EQId:
                        ID_state = id_Finish;
                        break;

                    case Setting_EQBaudrate:
                        BR_state = br_Finish;
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
                    switch ( routine_State )
                    {
                    case Routine_EQId:
                        ID_state = id_Finish;
                        break;

                    case Routine_EQData:
                        EQ_state = eq_Finish;
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

        for ( int i = 0; i < BUFFER_SIZE; i++ )     printf ( "data[%d] = %X;\n", i, * ( recv_data + i ) );
    }

}

void ProcessData ( int tid )
{
    switch ( main_state )
    {
    case Main_Setting:
        switch ( setting_state )
        {
        case Setting_EQId:
            switch ( ID_state )
            {
            case id_SetCmd:
                memset ( temp_char, 0, BUFFER_SIZE * sizeof ( char ) );
                cmd = 0x32;
                parameter = 0x00;
                isNeedSetCmd = true;
                ID_state = id_WaitData;
                break;

            case id_WaitData:
                if ( isCorrectData )
                {
//                    printf ( "recv_data: %X\n", * ( recv_data + 2 ) );

                    if ( * ( recv_data + 2 ) == 0xFF )
                    {
                        sprintf ( temp_char, "%X", * ( recv_data + 2 ) );
                        Global::isIdExist[tid] = false;
                    }
                    else
                    {
                        char *pch_sn = ( char* ) malloc ( 4 * sizeof ( char ) );
                        memmove ( pch_sn, recv_data + 3, 4 );
//                        for ( int i = 0; i < 4; i++ )    printf ( "pch_sn[%d] = %X;\n", i, * ( pch_sn + i ) );
                        sprintf ( temp_char, "%X", * ( pch_sn + 3 ) );
                        int pint_sn = atoi ( temp_char );
                        sprintf ( temp_char, "SN%05d", pint_sn );
                        free ( pch_sn );
                        Global::isIdExist[tid] = true;
                    }

                    isCorrectData = false;
                    ID_state = id_Finish;
                }

                break;

            case id_Finish:
                Global::isDataUpdate[tid] = true;

                if  ( !Global::isProcessUpdate[tid] )
                {
                    Global::temp_EQIdReturn_cmd.PortID[tid] = temp_char;
                    Global::isSubDataReady[tid] = DataReady;
                    ID_state = id_None;
                }

                Global::isDataUpdate[tid] = false;

                break;

            default:
                break;
            }

            break;

        case Setting_EQBaudrate:
            switch ( BR_state )
            {
            case br_SetCmd:
                memset ( temp_char, 0, 32 * sizeof ( char ) );
                cmd = 0x31;
                parameter = ParaBaudrate ( tid );
                isNeedSetCmd = true;
                BR_state = br_WaitData;
                break;

            case br_WaitData:
                if ( isCorrectData )
                {
                    if ( * ( recv_data + 2 ) == 0xAA )
                    {
                        sprintf ( temp_char, "%X", * ( recv_data + 2 ) );
                    }
                    else
                    {
                        sprintf ( temp_char, "%X", 0xDD );
                    }

                    isCorrectData = false;
                    BR_state = br_Finish;
                }

                break;

            case br_Finish:
                Global::isDataUpdate[tid] = true;

                if  ( !Global::isProcessUpdate[tid] )
                {
                    if ( strcmp ( temp_char, "AA" ) == 0 )    Global::temp_EQBaudrateReturn_cmd.PortSetting[tid] = 1;
                    else                                        Global::temp_EQBaudrateReturn_cmd.PortSetting[tid] = 0;

                    Global::isSubDataReady[tid] = DataReady;
                    BR_state = br_None;
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
        switch ( ID_state )
        {
        case id_SetCmd:
            printf ( "Main_Routine.....id_SetCmd..........\n" );
            memset ( temp_char, 0, BUFFER_SIZE * sizeof ( char ) );
            cmd = 0x32;
            parameter = 0x00;
            isNeedSetCmd = true;
            ID_state = id_WaitData;
            break;

        case id_WaitData:
            if ( isCorrectData )
            {
                printf ( "recv_data: %X\n", * ( recv_data + 2 ) );

                if ( * ( recv_data + 2 ) == 0xFF )
                {
                    sprintf ( temp_char, "%X", * ( recv_data + 2 ) );
                    Global::isIdExist[tid] = false;
                }
                else
                {
                    char *pch_sn = ( char* ) malloc ( 4 * sizeof ( char ) );
                    memmove ( pch_sn, recv_data + 3, 4 );
                    //for ( int i = 0; i < 4; i++ )    printf ( "pch_sn[%d] = %X;\n", i, * ( pch_sn + i ) );
                    sprintf ( temp_char, "%X", * ( pch_sn + 3 ) );
                    int pint_sn = atoi ( temp_char );
                    sprintf ( temp_char, "SN%05d", pint_sn );
                    free ( pch_sn );
                    Global::isIdExist[tid] = true;
                }

                isCorrectData = false;
                ID_state = id_Finish;
            }

            break;

        case id_Finish:
            Global::isRoutineIdUpdate[tid] = true;

            if  ( !Global::isProcessEqUpdate[tid] )
            {
                strcpy (  Global::temp_EQIdReturn_cmd.PortID[tid] , temp_char );
                ID_state = id_NextCmd;
            }

            Global::isRoutineIdUpdate[tid] = false;
            break;

        case id_NextCmd:
            // init EQ_state
            cmd_loop_total = 0;
            cmd_loop_current = 0;
            EQ_state = eq_SetCmd;
            memset ( serial_data, 0, 256 * sizeof ( char ) );

            ID_state = id_None;
            break;

        default:
            break;
        }

        //Global::SettingProcedure >=0x0F;
        switch ( EQ_state )
        {
        case eq_SetCmd:
            printf ( "Main_Routine......eq_SetCmd........\n" );
            cmd = 0x33;
            parameter = ParaEqData ( tid );
            isNeedSetCmd = true;
            memset ( temp_data, 0, 1024 * sizeof ( char ) );
            EQ_state = eq_WaitData;
            break;

        case eq_WaitData:
            if ( isCorrectData )
            {
                //for ( int i = 0; i < 32; i++ )      printf ( "recv_data[%d]: %02X\n", i, * ( recv_data + i ) );

                memmove ( serial_data + ( cmd_loop_current - 1 ) * 28, recv_data + 2, 28 );

                if ( cmd_loop_current >= cmd_loop_total )
                {

                    ParseData ( tid );

                    EQ_state = eq_Finish;
                    printf ( "EQ_state = %d \n", EQ_state );
                }
                else
                {
                    isCorrectData = false;
                    EQ_state = eq_SetCmd;
                }
            }

            break;

        case eq_Finish:
            printf ( "eq_Finish........\n" );
            Global::isRoutineEqUpdate[tid] = true;

            if  ( !Global::isProcessEqUpdate[tid] )
            {
                strcpy ( Global::temp_EQDataReturn_cmd.Data[tid] , temp_data );
                printf ( "temp_eq = %s\n", Global::temp_EQDataReturn_cmd.Data[tid] );
                EQ_state = eq_NextCmd;
            }

            Global::isRoutineEqUpdate[tid] = false;

            break;

        case eq_NextCmd:
            // init ID_state
            ID_state = id_SetCmd;

            EQ_state = eq_None;
            break;

        default:
            break;
        }

        break;

    default:
        break;
    }
}
void OutputData()
{
    if ( isNeedSetCmd )
    {
        if  ( true == SendMsg ( cmd, parameter ) )
        {
            memset ( recv_data, 0, BUFFER_SIZE * sizeof ( char ) );
            time_out = 20;
            isNeedGetData = true;
            isCorrectData = false;
            isNeedSetCmd = false;
        }
    }
}

void *DataCapture ( void *threadid )
{
    int  tid = ( int ) threadid;

    if ( !Global::CommunicationOpened )
        Global::CommunicationOpened = TreminalCommunicationOpen(tid);

    //SendMsg ( 0x31, 0x0D );
    //SendMsg ( 0x32, 0x00 );
    //SendMsg ( 0x33, 0x00 );   //KCM
    //SendMsg ( 0x33, 0x01 );   //SDC15 0x01,0x11,0x21,0x31,0x41.
    //SendMsg ( 0x33, 0x02 );   //HUSKY 0x02,0x12,0x22,0x32,0x42,0x52,0x62,0x72,0x82.
    //SendMsg ( 0x33, 0x06 );   //HUSKY 0x06,0x16,0x26,0x36,0x46,0x56,0x66,0x76,0x86.

    printf ( "Thread ID : %d\n", tid );

    while ( false == Global::isExitThread )
    {
        //printf("InputData\n");
        InputData ( tid );
        //printf("ProcessData\n");
        ProcessData ( tid );
        //printf("OutputData\n");
        OutputData();

        usleep ( 1000000 );
//        Global::EQBaudrateReturn_cmd.PortSetting[11]++;
    }
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

    switch ( port_type )
    {
    case KCM:
        cmd_loop_total = 1;
        ret = 0x00;
        break;

    case SDC15:
        cmd_loop_total = 5;
        ret = 0x01;
        break;

    case HUSKY:
        cmd_loop_total = 9;
        ret = 0x02;
        break;

    default:
        break;
    }

    ret += seq[cmd_loop_current];
    cmd_loop_current++;
//    printf ( "tid = %d\n", tid );
//    printf ( "port_type = %d\n", port_type );
//    printf ( "Global::EQType_cmd.PortType[tid] = %d\n", Global::EQType_cmd.PortType[tid] );
//    printf ( "cmd_loop_total = %d\n", cmd_loop_total );
    printf ( "ret = %X\n", ret );
    return ret;
}

void ParseData ( int tid )
{
    //for ( int i = 0; i < 127; i++ )      printf ( "serial_data[%d]: %02X     serial_data[%d]: %02X\n", 2 * i, * ( serial_data + 2 * i ), 2 * i + 1, * ( serial_data + 2 * i + 1 ) );

    int cmd_num = 0, cmd_size = 0;
    char *split_data = ( char* ) malloc ( 2 * BUFFER_SIZE * sizeof ( char ) );
    //char split_data[2 * BUFFER_SIZE] ={0};

    switch ( port_type )
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

    //sprintf(temp_data,"a");
    if ( KCM == port_type )
    {
        for ( int i = 0; i < cmd_num; i++ )
        {
            MainState state = Global::enum_MainState;
            memset ( split_data, 0, 2 * BUFFER_SIZE * sizeof ( char ) );
            sprintf ( split_data, "@%02X", * ( serial_data + ( cmd_size  ) * i ) );

            for ( int j = 0; j < cmd_size - 1; j++ )
            {
                char temp[10] = {'\0'};
                sprintf ( temp, "-%02X", * ( serial_data + ( ( cmd_size ) * i ) + 1 + j ) );
                strcat ( split_data, temp );
            }
            strcat ( temp_data, split_data ) ;
            Global::enum_MainState = state;
        }
    }
    else
    {
        for ( int i = 0; i < cmd_num; i++ )
        {
//            MainState state = Global::enum_MainState;
            memset ( split_data, 0, 2 * BUFFER_SIZE * sizeof ( char ) );
            sprintf ( split_data, "@%02X", * ( serial_data + ( cmd_size + 1 ) * i + 1 ) );

            for ( int j = 0; j < cmd_size - 1; j++ )
            {
                char temp[10] = {'\0'};
                sprintf ( temp, "-%02X", * ( serial_data + ( ( cmd_size + 1 ) * i + 1 ) + 1 + j ) );
                strcat ( split_data, temp );
            }

//
//        printf ( "Global::enum_MainState = %d\n", Global::enum_MainState );
        printf ( "split_data(%d): %s\n", i,  split_data );
//
            strcat ( temp_data, split_data ) ;
//        printf ( "Global::enum_MainState = %d\n", Global::enum_MainState );
//            Global::enum_MainState = state;
        }
    }

    //
    free ( split_data );
    printf ( "temp_data: %s\n", temp_data );
}



