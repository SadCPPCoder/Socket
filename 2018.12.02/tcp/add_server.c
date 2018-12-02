// simple server: the server get two number from clients, and then reply the add result of the
// two numbers to client.
// author: SadCPPCoder
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
    // socket
    int server_sock = socket( AF_INET, SOCK_STREAM, 0 );

    // define socket address
    struct sockaddr_in server_addr;
    bzero( (char*)&server_addr, sizeof(server_addr) );
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons( ADD_SERVER_PORT );
    server_addr.sin_addr.s_addr = htonl( INADDR_ANY );

    // bind
    if( bind( server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1 )
    {
        perror( "bind failed!\n" );
        close( server_sock );
        return -1;
    }

    // listen
    while( 1 )
    {
	if( listen( server_sock, 5 ) == -1 )
	{
	    perror( "listen failed!\n" );
	    close( server_sock );
	    return -1;
	}

	// client socket
	double num[2] = { 0.0 };
	struct sockaddr_in client_addr;
	bzero( (char*)&client_addr, sizeof(client_addr) );
	socklen_t length = sizeof(client_addr);

	int connect_fd = 0;
	if( (connect_fd = accept( server_sock, (struct sockaddr*)&client_addr, &length ) ) < 0 )
	{
	    perror( "Conncet failed!\n" );
	    close( server_sock );
	    return -1;
	}

	int len = 0;
	if( (len = recv( connect_fd, (char*)&num, sizeof(num), 0 ) ) < 0 )
	{
	    perror( "receive data failed!\n" );
	    close( connect_fd );
	    close( server_sock );
	    return -1;
	}

	double result = num[0] + num[1];

	// send data to client
	if( (len = send( connect_fd, (char*)&result, sizeof(result), 0 ) ) < 0 )
	{
	    perror( "send result failed!\n" );
	    close( connect_fd );
	    close( server_sock );
	    return -1;
	}

	close( connect_fd );
    }
    close( server_sock );

    return 0;
}
