#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include "interpreter.h"
#include "Global.h"
#include "action.h"
#include "DataCapture.h"
#include "Communication.h"

#define PORT "5000"     // connection porttr
#define BACKLOG 10      // pending connections queue

using namespace std;

void sigchld_handler ( int s )
{
    while ( waitpid ( -1, NULL, WNOHANG ) > 0 );
}

// get sockaddr，IPv4或IPv6：
void *get_in_addr ( struct sockaddr *sa )
{
    if ( sa->sa_family == AF_INET )
    {
        return & ( ( ( struct sockaddr_in* ) sa )->sin_addr );
    }

    return & ( ( ( struct sockaddr_in6* ) sa )->sin6_addr );
}

int main()
{
//    cout << "Welcome to new SWC!" << endl;
//    int sockfd, new_fd; // 在 sock_fd 進行 listen，new_fd 是新的連線
//    struct addrinfo hints, *servinfo, *p;
//    struct sockaddr_storage their_addr; // 連線者的位址資訊
//    socklen_t sin_size;
//    struct sigaction sa;
//    int yes = 1;
//    char s[INET6_ADDRSTRLEN];
//    int rv;
//
//    memset ( &hints, 0, sizeof hints );
//    hints.ai_family = AF_UNSPEC;
//    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_flags = AI_PASSIVE; // 使用我的 IP
//
//    if ( ( rv = getaddrinfo ( NULL, PORT, &hints, &servinfo ) ) != 0 )
//    {
//        fprintf ( stderr, "getaddrinfo: %s\n", gai_strerror ( rv ) );
//        return 1;
//    }
//
//    // use for loop to find all results，and bind（bind）the first capable result
//    for ( p = servinfo; p != NULL; p = p->ai_next )
//    {
//        if ( ( sockfd = socket ( p->ai_family, p->ai_socktype, p->ai_protocol ) ) == -1 )
//        {
//            perror ( "server: socket" );
//            continue;
//        }
//
//        if ( setsockopt ( sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof ( int ) ) == -1 )
//        {
//            perror ( "setsockopt" );
//            exit ( 1 );
//        }
//
//        if ( bind ( sockfd, p->ai_addr, p->ai_addrlen ) == -1 )
//        {
//            perror ( "server: bind" );
//            close ( sockfd );
//            continue;
//        }
//
//        break;
//    }
//
//    if ( p == NULL )
//    {
//        fprintf ( stderr, "server: failed to bind\n" );
//        return 2;
//    }
//
//    freeaddrinfo ( servinfo ); // 全部都用這個 structure
//
//    if ( listen ( sockfd, BACKLOG ) == -1 )
//    {
//        perror ( "listen" );
//        exit ( 1 );
//    }
//
//    sa.sa_handler = sigchld_handler; // 收拾全部死掉的 processes
//    sigemptyset ( &sa.sa_mask );
//    sa.sa_flags = SA_RESTART;
//
//    if ( sigaction ( SIGCHLD, &sa, NULL ) == -1 )
//    {
//        perror ( "sigaction" );
//        exit ( 1 );
//    }
//
//    printf ( "server: waiting for connections...\n" );
//
//    while ( 1 ) // main loop: accept()
//    {
//        sin_size = sizeof their_addr;
//        new_fd = accept ( sockfd, ( struct sockaddr * ) &their_addr, &sin_size );
//
//        if ( new_fd == -1 )
//        {
//            perror ( "accept" );
//            continue;
//        }
//
//        inet_ntop ( their_addr.ss_family, get_in_addr ( ( struct sockaddr * ) &their_addr ), s, sizeof s );
//        printf ( "server: got connection from %s\n", s );
//
//        if ( 0 == fork() ) // go into child process
//        {
//            close ( sockfd  ); // child 不需要 listener
//
//            // add something here
//            char buffer[2048] = "Hi Client!";
//            char buffer1[2048];
            pthread_t threads[1];
//            int interpreterloopcomplete = 0;
            bool ThreadCreated = false;
            int ret = 0;
//
//            // initialize
//            Global::SettingProcedure = 0;
//            Global::enum_MainState = Main_None;
//            Global::enum_SettingState = Setting_None;
//            Global::enum_RoutineState = Routine_None;

            //  Create thread
            Global::DataLatch = false;      // for test thread
            Global::SettingProcedure = 15;  // for test thread

            if ( ( ( Global::SettingProcedure & 15 ) == 15 ) && ( !ThreadCreated ) )
            {
                printf ( "ThreadCreated\n" );
                ThreadCreated = true;
                int thread_id = 0;

                if ( 0 != pthread_create ( &threads[0], NULL, DataCapture, ( void * ) thread_id ) )
                {
                    printf ( "Create pthread error!\n" );
                }
            }
            ///////

            while ( ret >= 0 )
            {
//                memset ( buffer1, 0, 100 );
//                memset ( buffer, 0, 100 );
//
//                ret = recv ( new_fd, buffer1, sizeof ( buffer1 ), 0 );
//
//                if ( -1 == ret )
//                {
//                    perror ( "recv" );
//                }
//                else
//                {
//                    interpreterloopcomplete = interpreter ( buffer1, ret );
//                    printf("aaaacacacaa\n");
//                    action ( buffer );
//                    printf ( "%s\n", buffer );
//
//                    if ( send ( new_fd, buffer, sizeof ( buffer ), 0 ) == -1 )
//                    {
//                        perror ( "send" );
//                    }
//                }
                usleep ( 1000000 );
            }

//            printf ( "Out of thread!\n" );
//            pthread_exit ( NULL );
//            // add end
//
//            close ( new_fd );
//        }
//        else    // back to parent process
//        {
//            close ( new_fd ); // parent 不需要這個
//        }
//    }

    return 0;
}

