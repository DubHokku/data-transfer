#include <iostream>

#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef SERVER_H
#define SERVER_H
#include "recipient.h"
#endif // SERVER_H

#ifndef CLIENT_H
#define CLIENT_H
#include "sender.h"
#endif // CLIENT_H

int main( int ac, char **av )
{
    if( ac < 2 )
    {
        std::cout << "type " << av[0] << " ( server | <data> <path> <server address> )" << std::endl;
        return 0;
    }
    
    if( ac > 3 )
    {
        sender_t client;
        client.path = av[ac - 2];
        client.server = av[ac - 1];
        int socket = client.start();
        
        int sequence = atoi( av[1]);
        for( int i = sequence, j = 2; j <= ac - 3; i++, j++ )
        {
            if( atoi( av[j]) != sequence + 1 )
            {
                sequence = atoi( av[j]);
                client.transfer( &socket, &sequence );
            }
            else
                sequence = atoi( av[j]);
        }
        
        client.stop( &socket );
        return 0;
    }
    
    if(( strcmp( av[1], "server" )) == 0 )
    {
        recipient_t server;
        server.receive();
    }
    else
        std::cout << "type " << av[0] << " ( server | <data> <path> <server address> )" << std::endl;
    
    return 0;
}
