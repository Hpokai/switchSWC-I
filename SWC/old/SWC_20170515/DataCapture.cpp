#include <ctime>
#include <cerrno>
#include <unistd.h>
#include "DataCapture.h"
#include "Communication.h"

void *DataCapture ( void *threadid )
{
    long tid;
    tid = ( long ) threadid;
    int i = 0;

    if ( !Global::CommunicationOpened )
    {
        Global::CommunicationOpened = TreminalCommunicationOpen();
    }

    while ( 1 )
    {
//  i++;
//  printf("Thread ID : %d\n", i);

        if ( !Global::DataLatch )
        {
            SendMsg();
            usleep ( 100000 );
            ReadMsg();
        }

        Global::EQBaudrateReturn_cmd.PortSetting[11]++;
    }

}
