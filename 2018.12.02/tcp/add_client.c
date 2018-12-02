// simple client: send two number to server, and the server reply the add result.
// author: SanCPPCoder
// date: 2018.12.02

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define ADD_SERVER_PORT 	( 8888 )
#define ADD_SERVER_IP		( "127.0.0.1" )
#define INPUT_COUNT		( 3 )

int main(int argc, char **argv)
{
    // get the input
    if( INPUT_COUNT != argc )
    {
        perror( "Error input!\nPlease input like:\n\t\t client numberA numberB\n" );
        return -1;
    }

    double num[2] = { 0.0 };
    num[0] = atof( &argv[1][0] );
    num[1] = atof( &argv[2][0] );

    printf( "The input data is [%f] and [%f].\n", num[0], num[1] );

    // socket
    int client_sock = socket( AF_INET, SOCK_STREAM, 0 ); // IPV4, Stream, TCP

    // define the socket address of server
    struct sockaddr_in server_addr;
    bzero( &server_addr, sizeof(server_addr) );
    server_addr.sin_family = AF_INET; // IPV4
    server_addr.sin_port = htons( ADD_SERVER_PORT ); // server port
    server_addr.sin_addr.s_addr = inet_addr( ADD_SERVER_IP ); // server ip

    // connect
    if( connect( client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr) ) < 0 )
    {
        perror( "Connect to server failed!\n" );
        // TODO: output error information.
        close( client_sock );
        return -1;
    }
    
    // send
    int send_len = 0;
    if( (send_len = send( client_sock, (char*)&num, sizeof(num), 0 ) ) <= 0 )
    {
        perror( "Send data failed!\n" );
        close( client_sock );
        // TODO: resend data
        return -1;
    }

    // recv
    double result = 0.0;
    if( (send_len = recv( client_sock, (char*)&result, sizeof(result), 0 ) ) < 0 )
    {
        perror( "receive data failed!\n" );
        close( client_sock );
        return -1;
    }

    printf( "Receive the result of [%f] + [%f] = [%f].\n", num[0], num[1], result );

    // close
    close( client_sock );
}
