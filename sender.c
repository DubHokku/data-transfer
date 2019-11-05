#include "sender.h"

sender_t::sender_t()
{
    port = 127;
}
sender_t::~sender_t(){}

int sender_t::transfer( int *socket, int *sequence )
{
    msg_t* msg = new msg_t;
    memset( msg, 0, sizeof( *msg ));
    
    msg->data = *sequence;
    strcpy( msg->path, path );
    
    std::cout << "transfer " << *socket << " sequence " << *sequence << " " << path << " to " << server << std::endl;
    send( *socket, msg, sizeof( *msg ), 0 );
    
    delete( msg );
    
    return 0;
}

int sender_t::start()
{
    int descriptor = socket( AF_INET, SOCK_STREAM, 0 );
    if( descriptor < 0 )
        notify( "socket()", errno );

    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_addr.s_addr = inet_addr( server );
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons( port );

    if( connect( descriptor, ( struct sockaddr* )&server_sockaddr, sizeof( server_sockaddr )) < 0 )
        notify( "connect()", errno );

    return descriptor;
}

int sender_t::stop( int *socket )
{
    return shutdown( *socket, 1 );
}
void sender_t::notify( const char* func, int code )
{
    std::cout << func << ": " << strerror( code ) << std::endl;
    exit( 0 );
}
