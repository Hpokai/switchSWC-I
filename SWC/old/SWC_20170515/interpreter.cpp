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

    int type = 0;

    int step = 0;
    char *FiltCmd;
    char *FiltCmd1[128];
    char *FiltCmd2;
    char *FiltCmd3[128];
    char *tempFiltCmd, *tempFiltCmd1, *tempFiltCmd2;

    const char *char_star = "*";
    const char *char_hash = "#";
    const char *char_comma = ",";
    const char *char_at = "@";
    const char *char_at_hash = "@#";
    const char *char_dollar = "$";
    const char *char_ncolon = "N:";
    const char *char_colon_hash = ":#";

    char cmdtype[30] = "";
    bool loop = true;

//  memset(FiltCmd,0,1);
//  memset(FiltCmd1,0,128);

    for ( int i = 0; i < (int)cmdsize; i++ )
    {
        printf ( "cmdsize%d\n", cmdsize );
        string charstr ( 1, cmd[0] );

        if ( step == 0 )
        {
            FiltCmd = strtok ( cmd, char_star );
            strcpy ( cmdtype, FiltCmd );
            FiltCmd = strtok ( NULL, char_star );
            printf ( "%s\n", FiltCmd );

            step = 1;
        }
    }

    if ( step == 1 )
    {
        if ( strcmp ( cmdtype, "EQBaudrate" ) == 0 )
        {

            Global::RqtType = BaudrateRequest;
            Global::SettingProcedure = Global::SettingProcedure | 1;
            int cmdnum = 0;
            printf ( "result = %s\n", cmdtype );
            printf ( "%s\n", FiltCmd );

            tempFiltCmd = strtok ( FiltCmd, char_hash );

            while ( tempFiltCmd )
            {
                FiltCmd1[cmdnum] = tempFiltCmd;
                cmdnum++;
                tempFiltCmd = strtok ( NULL, char_hash );
            }

//        printf("dsdfdf = %d\n",cmdnum);
            int index = 0;
            printf ( "cmdnum = %d\n", cmdnum );

            for ( int j = 0; j < cmdnum - 1; j++ )
            {
                index = 0;
                char dataformatbuf[] = "";
                printf ( "FiltCmd1[j] = %s\n", FiltCmd1[j] );
                tempFiltCmd2 = strtok ( FiltCmd1[j], char_comma );

                while ( tempFiltCmd2 )
                {
                    printf ( "%s", "9999999999999\n" );

                    if ( tempFiltCmd2 != "%" )
                    {
                        if ( index == 1 )
                        {
                            Global::EQBaudrate_cmd.baurate[j] = atoi ( tempFiltCmd2 );
                            printf ( "BR = %d\n", Global::EQBaudrate_cmd.baurate[j] );
                        }
                        else if ( ( index > 1 ) && ( index <= 4 ) )
                        {
                            strcat ( dataformatbuf, tempFiltCmd2 );
//              printf("dataformat = %s\n", dataformatbuf);
                            Global::EQBaudrate_cmd.dataformat[j] = dataformatbuf;
                        }
                        else
                        {
                            ;
                        }
                    }
                    else
                    {
                        ;
                    }

                    index++;
//           printf("Global::EQBaudrate_cmd.dataformat = %s\n",Global::EQBaudrate_cmd.dataformat[j]);
                    tempFiltCmd2 = strtok ( NULL, char_comma );
                }

                printf ( "j = %d\n", j );
                printf ( "Global::EQBaudrate_cmd.dataformat = %s\n", Global::EQBaudrate_cmd.dataformat[j] );
            }

            printf ( "%s\n", "4444444448" );
        }
        else if ( strcmp ( cmdtype, "EQId" ) == 0 )
        {

            Global::RqtType = IdRequest;
            Global::SettingProcedure = Global::SettingProcedure | 2;
            int cmdnum = 0;
//        char IDBuf[] ="";
            printf ( "%s\n", FiltCmd );

            tempFiltCmd = strtok ( FiltCmd, char_at );

            while ( tempFiltCmd )
            {
                printf ( "tempFiltCmd = %s\n", tempFiltCmd );
                FiltCmd2 = tempFiltCmd;
                cmdnum++;
                tempFiltCmd = strtok ( NULL, char_at );
            }

            tempFiltCmd2 = strtok ( FiltCmd2, char_hash );
            Global::EQId_cmd.IP = tempFiltCmd2;
            printf ( "Global::EQId_cmd.IP = %s\n", Global::EQId_cmd.IP );

            while ( tempFiltCmd2 )
            {
                if ( cmdnum >= 1 )
                {
                    FiltCmd1[cmdnum] = tempFiltCmd2;
                }

                cmdnum++;
                tempFiltCmd2 = strtok ( NULL, char_hash );
            }

            for ( int j = 0; j < cmdnum - 1; j++ )
            {
                int i = 0;
                tempFiltCmd2 = strtok ( FiltCmd1[j + 1], char_comma );

                while ( tempFiltCmd2 )
                {
                    if ( i == 1 )
                    {
                        //            strcpy(IDBuf,tempFiltCmd2);
                        Global::EQId_cmd.ID[j] = tempFiltCmd2;
                        printf ( "Num = %d ,Global::EQId_cmd.ID = %s\n", j, Global::EQId_cmd.ID[j] );
                    }

                    i++;
                    tempFiltCmd2 = strtok ( NULL, char_comma );
                }
            }
        }
        else if ( strcmp ( cmdtype, "EQType" ) == 0 )
        {
            int cmdnum = 0;
            int i;
            printf ( "%s\n", FiltCmd );
            Global::SettingProcedure = Global::SettingProcedure | 4;
            tempFiltCmd = strtok ( FiltCmd, char_at_hash );

            while ( tempFiltCmd )
            {
                printf ( "tempFiltCmd = %s\n", tempFiltCmd );
                FiltCmd1[cmdnum] = tempFiltCmd;
                cmdnum++;
                tempFiltCmd = strtok ( NULL, char_at_hash );
            }

            for ( i = 0; i < 3; i++ )
            {
                cmdnum = 0;
                tempFiltCmd = strtok ( FiltCmd1[i], char_dollar );

                while ( tempFiltCmd )
                {
                    //   printf("tempFiltCmd = %s\n", tempFiltCmd);
                    if ( cmdnum == 0 )
                    {
                        Global::EQType_cmd.CmdType[i] = tempFiltCmd;
                        printf ( "Global::EQType_cmd.CmdType[%d] = %s\n", i, Global::EQType_cmd.CmdType[i] );
                    }
                    else
                    {
                        FiltCmd3[i] = tempFiltCmd;
                        printf ( "FiltCmd3[%d] = %s\n", i, FiltCmd3[i] );
                    }

                    //        FiltCmd1[cmdnum]=tempFiltCmd;
                    cmdnum++;
                    tempFiltCmd = strtok ( NULL, char_at_hash );
                }
            }

            for ( i = 0; i < 3; i++ )
            {
                cmdnum = 0;
                tempFiltCmd = strtok ( FiltCmd3[i], char_ncolon );

                while ( tempFiltCmd )
                {
                    //   printf("tempFiltCmd = %s\n", tempFiltCmd);
                    if ( cmdnum == 0 )
                    {
                        Global::EQType_cmd.CmdNum[i] = atoi ( tempFiltCmd );
                        printf ( "Global::EQType_cmd.CmdNum[%d] = %d\n", i, Global::EQType_cmd.CmdNum[i] );
                    }
                    else
                    {
                        FiltCmd1[i] = tempFiltCmd;
                        printf ( "FiltCmd1[%d] = %s\n", i, FiltCmd1[i] );
                    }

                    //        FiltCmd1[cmdnum]=tempFiltCmd;
                    cmdnum++;
                    tempFiltCmd = strtok ( NULL, char_ncolon );
                }

            }

            for ( i = 0; i < 3; i++ )
            {
                tempFiltCmd = strtok ( FiltCmd1[i], char_comma );

                while ( tempFiltCmd )
                {
                    if ( strcmp ( Global::EQType_cmd.CmdType[i], "KCM" ) == 0 )
                    {
                        Global::EQType_cmd.PortType[atoi ( tempFiltCmd )] = KCM;
                    }
                    else if ( strcmp ( Global::EQType_cmd.CmdType[i], "HUSKY" ) == 0 )
                    {
                        Global::EQType_cmd.PortType[atoi ( tempFiltCmd )] = HUSKY;
                    }
                    else if ( strcmp ( Global::EQType_cmd.CmdType[i], "SDC15" ) == 0 )
                    {
                        Global::EQType_cmd.PortType[atoi ( tempFiltCmd )] = SDC15;
                    }

                    printf ( "Global::EQType_cmd.PortType[%d] = %d\n", atoi ( tempFiltCmd ), Global::EQType_cmd.PortType[atoi ( tempFiltCmd )] );
                    tempFiltCmd = strtok ( NULL, char_comma );
                }
            }
        }
        else if ( strcmp ( cmdtype, "EQCmd" ) == 0 )
        {
            int cmdnum = 0;
            int KCMCmdNum = 0;
            int HUSKYCmdNum = 0;
            int SDC15CmdNum = 0;
            int cmdindex = 0;
            char *cmdtype;
            //      printf("%s\n",FiltCmd);
            Global::SettingProcedure = Global::SettingProcedure | 8;
            tempFiltCmd = strtok ( FiltCmd, char_at );

            while ( tempFiltCmd )
            {
                printf ( "tempFiltCmd = %s\n", tempFiltCmd );
                FiltCmd1[cmdnum] = tempFiltCmd;
                cmdnum++;
                tempFiltCmd = strtok ( NULL, char_at );
            }

            for ( int i = 0; i < cmdnum; i++ )
            {
                cmdindex = 0;
                tempFiltCmd = strtok ( FiltCmd1[i], char_colon_hash );

                while ( tempFiltCmd )
                {
                    //  printf("tempFiltCmd = %s\n", tempFiltCmd);
                    if ( cmdindex == 0 )
                    {
                        cmdtype = tempFiltCmd;
                    }
                    else if ( cmdindex == 1 )
                    {
                        if ( strcmp ( cmdtype, "KCM" ) == 0 )
                        {
                            Global::EQCmd_cmd.KCMCmd[KCMCmdNum] = tempFiltCmd;
                            printf ( "Cmd %d , KCMCmd[%d] = %s\n", i, KCMCmdNum, Global::EQCmd_cmd.KCMCmd[KCMCmdNum] );
                            KCMCmdNum++;
                        }
                        else if ( strcmp ( cmdtype, "HUSKY" ) == 0 )
                        {
                            Global::EQCmd_cmd.HUSKYCmd[HUSKYCmdNum] = tempFiltCmd;
                            printf ( "Cmd %d , HUSKYCmd[%d] = %s\n", i, HUSKYCmdNum, Global::EQCmd_cmd.HUSKYCmd[HUSKYCmdNum] );
                            HUSKYCmdNum++;
                        }
                        else if ( strcmp ( cmdtype, "SDC15" ) == 0 )
                        {
                            Global::EQCmd_cmd.SDC15Cmd[SDC15CmdNum] = tempFiltCmd;
                            printf ( "Cmd %d , SDC15Cmd[%d] = %s\n", i, SDC15CmdNum, Global::EQCmd_cmd.SDC15Cmd[SDC15CmdNum] );
                            SDC15CmdNum++;
                        }
                    }

                    cmdindex++;
                    tempFiltCmd = strtok ( NULL, char_colon_hash );
                }


            }

        }
        else if ( strcmp ( cmdtype, "EQData" ) == 0 )
        {

            Global::RqtType = DataRequest;

            int cmdnum = 0;
            printf ( "%s\n", FiltCmd );

            tempFiltCmd = strtok ( FiltCmd, char_hash );

            while ( tempFiltCmd )
            {
                if ( cmdnum == 0 )
                {
                    Global::EQData_cmd.CmdNum = atoi ( tempFiltCmd );
                    printf ( "tempFiltCmd = %d\n", Global::EQData_cmd.CmdNum );

                }

                // printf("tempFiltCmd = %s\n", tempFiltCmd);
                //        FiltCmd1[cmdnum]=tempFiltCmd;
                cmdnum++;
                tempFiltCmd = strtok ( NULL, char_hash );
            }

        }
        else
        {
            ;
        }

        step = 2;
        printf ( "%s\n", "aaaaaaaaaaaa" );
    }

    if ( step == 2 )
    {

    }
    else
    {
        ;
    }

    printf ( "%s\n", "88888888" );

    type = 1;


    return type;
}
