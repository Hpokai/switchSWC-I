#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <cerrno>
#include <unistd.h>
#include "ProcessControl.h"

using namespace std;

MainState main_state_in_process = Main_None;
SettingState setting_state_in_process = Setting_None;
RoutineState routine_State_in_process = Routine_None;

bool isInputEnable = true;
bool isFinish = false;
int routine_count = 0;

void InputState()
{
    if ( true == isInputEnable )
    {
        if ( main_state_in_process != Global::enum_MainState )
        {
            main_state_in_process = Global::enum_MainState;
        }

        switch ( main_state_in_process )
        {
        case Main_Setting:
            if ( setting_state_in_process != Global::enum_SettingState )
            {
                setting_state_in_process = Global::enum_SettingState;
                isFinish = false;
            }

            break;

        case Main_Routine:
            if ( routine_State_in_process != Global::enum_RoutineState )
            {
                routine_State_in_process = Global::enum_RoutineState;
                isFinish = false;
            }

            break;

        default:
            break;
        }

    }
}
void ProcessState()
{
    switch ( main_state_in_process )
    {
    case Main_Setting:
        switch ( setting_state_in_process )
        {
        case Setting_EQId:
            if ( !isFinish )
            {
                int sum = 0;

                for ( int i = 0; i < MAX_CONNECTOR; i++ )
                {
                    Global::isProcessUpdate[i] = true;

                    if ( ( !Global::isDataUpdate[i] ) && ( !Global::isActionUpdate ) )
                    {
                        if ( DataReady == Global::isSubDataReady[i] )
                        {
                            Global::EQIdReturn_cmd.PortID[i] = Global::temp_EQIdReturn_cmd.PortID[i];
                            Global::isSubDataReady[i] = DataGotten;
                        }
                    }

                    Global::isProcessUpdate[i] = false;

                    if ( DataGotten == Global::isSubDataReady[i] )  sum++;
                }

                if ( sum >= MAX_CONNECTOR )
                {
                    Global::isDataReady = true;
                    isFinish = true;
                }
            }

            break;

        case Setting_EQBaudrate:
            if ( !isFinish )
            {
                int sum = 0;

                for ( int i = 0; i < MAX_CONNECTOR; i++ )
                {
                    Global::isProcessUpdate[i] = true;

                    if ( ( !Global::isDataUpdate[i] ) && ( !Global::isActionUpdate ) )
                    {
                        if ( DataReady == Global::isSubDataReady[i] )
                        {
                            Global::EQBaudrateReturn_cmd.PortSetting[i] = Global::temp_EQBaudrateReturn_cmd.PortSetting[i];
                            Global::isSubDataReady[i] = DataGotten;
                        }
                    }

                    Global::isProcessUpdate[i] = false;

                    if ( DataGotten == Global::isSubDataReady[i] )  sum++;
                }

                if ( sum >= MAX_CONNECTOR )
                {
                    Global::isDataReady = true;
                    isFinish = true;
                }
            }

            break;

        case Setting_EQType:
            if ( !isFinish )
            {
                Global::isDataReady = true;
                isFinish = true;
            }

            break;

        case Setting_EQCmd:
            if ( !isFinish )
            {
                Global::isDataReady = true;
                isFinish = true;
            }

            break;

        default:
            break;
        }

        break;

    case Main_Routine:
        for ( int i = 0; i < MAX_CONNECTOR; i++ )
        {
            Global::isProcessIdUpdate[i] = true;

            if ( ( !Global::isRoutineIdUpdate[i] ) && ( !Global::isActionUpdate ) )
            {
                Global::EQIdReturn_cmd.PortID[i] = Global::temp_EQIdReturn_cmd.PortID[i];
            }

            Global::isProcessIdUpdate[i] = false;
        }

        for ( int i = 0; i < MAX_CONNECTOR; i++ )
        {
            Global::isProcessEqUpdate[i] = true;

            if ( ( !Global::isRoutineEqUpdate[i] ) && ( !Global::isActionUpdate ) )
            {
              Global::EQDataReturn_cmd.Data[i] = Global::temp_EQDataReturn_cmd.Data[i];
            }

            Global::isProcessEqUpdate[i] = false;
        }


        if ( !isFinish )
        {
            Global::isDataReady = true;
            isFinish = true;
        }

        break;

    default:
        break;
    }
}
void OutputState()
{
}

void *ProcessControl ( void *arg )
{
    cout << "Welcome to ProcessControl" << endl;

    while ( false == Global::isExitThread )
    {
        //printf("InputState\n");
        InputState();
        //printf("ProcessState\n");
        ProcessState();
        //printf("OutputState\n");
        OutputState();
//        switch ( Global::enum_MainState )
//        {
//        case Main_Setting:
//            break;
//
//        case Main_Routine:
//            break;
//
//        default:
//            break;
//        }
        usleep ( 150000 );
    }
    //pthread_exit(NULL);
    return NULL;
}
