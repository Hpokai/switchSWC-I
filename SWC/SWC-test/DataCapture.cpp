#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <cerrno>
#include <unistd.h>
#include "DataCapture.h"
#include "Communication.h"

void *DataCapture ( void *threadid )
{
    int  tid = ( int ) threadid;

    if ( !Global::CommunicationOpened )     Global::CommunicationOpened = TreminalCommunicationOpen();

    //SendMsg ( 0x31, 0x0D );
    //SendMsg ( 0x32, 0x00 );
    //SendMsg ( 0x33, 0x00 );   //KCM
    //SendMsg ( 0x33, 0x01 );   //SDC15 0x01,0x11,0x21,0x31,0x41.
    //SendMsg ( 0x33, 0x02 );   //HUSKY 0x02,0x12,0x22,0x32,0x42,0x52,0x62,0x72,0x82.

    while ( 1 )
    {
        printf ( "Thread ID : %d\n", tid );

        if ( !Global::DataLatch )
        {
            SendMsg ( 0x33, 0x01 );
        }

        usleep ( 5000000 );

        if ( !Global::DataLatch )
        {
            ReadMsg();
        }

        usleep ( 5000000 );
//        Global::EQBaudrateReturn_cmd.PortSetting[11]++;
    }

}
