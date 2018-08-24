#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include "Global.h"
#include "Definition.h"
#include "Communication.h"

#define loop_cnt    20000

unsigned long long TotBytes [4] = {0, 0, 0, 0};
unsigned long long err_cnt [4] = {0, 0, 0, 0};
unsigned long long cycle [4] = {0, 0, 0, 0};

int id[MAX_CONNECTOR] = {-1};
char slave_add[MAX_CONNECTOR] = {0x10, 0x12, 0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24, 0x26};


//char slave_add[MAX_CONNECTOR] = {0x10, 0x12, 0x14, 0x16, 0x18, 0x1C, 0x1E, 0x20, 0x22, 0x24, 0x26};

//char slave_add[MAX_CONNECTOR] = {0x10, 0x16, 0x18, 0x1A, 0x1C, 0x20, 0x22, 0x24, 0x26};

bool TerminalCommunicationOpen(int tid)
{
    bool opensuccessfully = true;
//    printf ( "Communication : %X\n", slave_add[tid] );

    if ( ( id[tid] = open ( "/dev/i2c-1", O_RDWR ) ) < 0 )
    {
        perror ( "Failed to open the bus\n" );
    }

    if ( ioctl ( id[tid], I2C_SLAVE, slave_add[tid] ) < 0 )
//    if ( ioctl ( id[tid], I2C_SLAVE, 0x1C) < 0 )
    {
        perror ( "Failed to connect to the sensor\n" );
    }
    // printf ( "i2c Device No#: 0x%X\n", slave_add[tid] );


    return opensuccessfully;
}

bool TerminalCommunicationClose(int tid)
{
    bool ret = false;

    close(id[tid]);

    return ret;
}


bool SendMsg ( int tid,char cmd, char parameter )
{

    printf ( "[Thread Id = %d]: SenMsg\n", tid );

    TerminalCommunicationOpen(tid);

    bool ret = false;
    char SendTbl[BUFFER_SIZE];
    memset(SendTbl, 0, BUFFER_SIZE);
    SendTbl[0] = cmd;
    SendTbl[1] = parameter;

    printf ( "[tid= %d ]Send %X, %X \n",tid,SendTbl[0],SendTbl[1] );
    int send_num = write ( id[tid], SendTbl, 2 );
    if ( 0 <  send_num)
    {
//        printf ( "[tid= %d ]Send Message OK!\n",tid );
        ret = true;
    }
    else if ( 0 == send_num)
    {
        printf ( "[tid= %d ]Send ZERO(((0)))!\n",tid );
    }
    else
    {
        printf ( "[tid= %d ]Send Error!\n",tid );
    }

    return ret;
}

char* ReadMsg(int tid)
{
    char GetTbl[BUFFER_SIZE];
    char* return_byte = ( char* ) malloc ( BUFFER_SIZE * sizeof ( char ) );

    memset(GetTbl, 0, BUFFER_SIZE);
    printf ( "[Thread Id = %d]: ReadMsg\n", tid );

//    if ( read ( id[tid], GetTbl, BUFFER_SIZE ) != BUFFER_SIZE )
    if ( read ( id[tid], GetTbl, BUFFER_SIZE ) != -1 )
    {
//        printf ( "Error Read Message! [%02d]\n",tid );
//    }
//    else
//    {
//        printf ( "[tid= %d ]Read Message OK!\n",tid );

        printf ( "RRRTTTEEE[%02d] = ",tid );

        for ( int i = 0; i < BUFFER_SIZE; i++ )
        {
            * ( return_byte + i ) = GetTbl[i];
            printf ( "(%X) ", *(return_byte+i) );
        }
        printf ( "\n" );
    }

    TerminalCommunicationClose(tid);
    return return_byte;
}
