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
#include "action.h"

#define BUFFER_SIZE 32
#define loop_cnt    20000

unsigned long long TotBytes [4] = {0, 0, 0, 0};
unsigned long long err_cnt [4] = {0, 0, 0, 0};
unsigned long long cycle [4] = {0, 0, 0, 0};
char SendTbl[BUFFER_SIZE];
char GetTbl[BUFFER_SIZE];
int Dadd[4] = {0x10, 0x12, 0x14, 0x16};
int SLen;

int id[12];
unsigned char DevR;

bool TreminalCommunicationOpen()
{
    bool opensuccessfully = false;

    printf ( "Communication : %s\n", "lalalalalalalalal" );

    for ( int i = 0; i < 12; i++ )
    {
        id[i] = 0;
    }

    DevR = ( int ) ( rand() % 4 );

    if ( ( id[0] = open ( "/dev/i2c-1", O_RDWR ) ) < 0 )
    {
        perror ( "Failed to open the bus\n" );
        return false;
    }

    if ( ioctl ( id[0], I2C_SLAVE, 0x10 ) < 0 )
//  if (ioctl(id[0], I2C_SLAVE, Dadd[DevR]) < 0)
    {
        perror ( "Failed to connect to the sensor\n" );
        return false;
    }

    printf ( "i2c Device No#: 0x%X\n", Dadd[DevR] );

    opensuccessfully = true;

    return true;
}

void SendMsg()
{
    SendTbl[0] = 0x12;

    if ( write ( id[0], SendTbl, 1 ) != 1 )
    {
        printf ( "Error Send Message!\n" );
        return;
    }

    printf ( "Send Message OK!\n" );
    return;
}

void ReadMsg()
{
    if ( read ( id[0], GetTbl, 8 ) != 8 )
    {
        printf ( "Error Read Message!\n" );
        return;
    }

    printf ( "Read Message OK!\n" );

    for ( int i = 0; i < 8; i++ )
    {
        printf ( "Msg = %X; ", GetTbl[i] );
    }

    printf ( "\n" );
    return;
}
