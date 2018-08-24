#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include "Global.h"
#include "Definition.h"
#include "Communication.h"

#define loop_cnt    20000

unsigned long long TotBytes [4] = {0, 0, 0, 0};
unsigned long long err_cnt [4] = {0, 0, 0, 0};
unsigned long long cycle [4] = {0, 0, 0, 0};
char SendTbl[BUFFER_SIZE];
char GetTbl[BUFFER_SIZE];
int SLen;

int id[MAX_CONNECTOR] = {-1};
char slave_add[MAX_CONNECTOR] = {0x10, 0x12, 0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24, 0x26};

bool TreminalCommunicationOpen(int tid)
{
    bool opensuccessfully = false;

    printf ( "Communication : %X\n", slave_add[tid] );

    if ( ( id[tid] = open ( "/dev/i2c-1", O_RDWR ) ) < 0 )
    {
        perror ( "Failed to open the bus\n" );
        return false;
    }

//    if ( ioctl ( id[tid], I2C_SLAVE, slave_add[tid] ) < 0 )
    if ( ioctl ( id[tid], I2C_SLAVE, 0x1C) < 0 )
    {
        perror ( "Failed to connect to the sensor\n" );
        return false;
    }

    printf ( "i2c Device No#: 0x%X\n", slave_add[tid] );

    opensuccessfully = true;

    return true;
}

bool SendMsg ( char cmd, char parameter )
{
    bool ret = false;
    SendTbl[0] = cmd;
    SendTbl[1] = parameter;

    if ( -1 != write ( id[0], SendTbl, 2 ) )
    {
        printf ( "Send Message OK!\n" );
        ret = true;
    }

    return ret;
}

char* ReadMsg()
{
    char* return_byte = ( char* ) malloc ( BUFFER_SIZE * sizeof ( char ) );

    if ( read ( id[0], GetTbl, BUFFER_SIZE ) != BUFFER_SIZE )
    {
        printf ( "Error Read Message!\n" );
    }
    else
    {
        printf ( "Read Message OK!\n" );

        for ( int i = 0; i < BUFFER_SIZE; i++ )
        {
            * ( return_byte + i ) = GetTbl[i];
            //printf ( "RTE[%d] = %X;\n", i, *(return_byte+i) );
        }
    }

    return return_byte;
}
