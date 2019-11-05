#include "recipient.h"

recipient_t::recipient_t()
{
    port = 127;
    session_data = new session_t;
    msg = new msg_t;
}

recipient_t::~recipient_t()
{
    delete session_data;
    delete msg;
}

void recipient_t::receive()
{
    int result, max_d_num;
    server_socket = start();
    
    char data[4344]; // mss x 3
    char ascii[12];
    
    char directory[512];
    memset( directory, 0, sizeof( directory ));
    getcwd( directory, sizeof( directory ));
    
    while( true )
    {
        fd_set sock_d_set;
        FD_ZERO( &sock_d_set );
        FD_SET( server_socket, &sock_d_set );
        max_d_num = server_socket;
        
        for( session = sessions.begin(); session != sessions.end(); session++ )
        {
            FD_SET( session->session_socket, &sock_d_set );
            max_d_num = session->session_socket > max_d_num?session->session_socket:max_d_num;
        }
        
        if(( select( max_d_num + 1, &sock_d_set, NULL, NULL, NULL )) < 0 )
        {
            if( errno == EINTR )
                continue;
            notify( "select()", errno );
        }
        
        for( session = sessions.begin(); session != sessions.end(); session++ )
        {
            result = recv( session->session_socket, ( void* )msg, sizeof( *msg ), 0 );
            if( result < 0 )
            {
                if(( errno != EINTR ) && ( errno != EAGAIN ) && ( errno != EWOULDBLOCK ))
                {
                    notify( "recv()", errno );
                    tmp_link = session;
                    session++;
                    
                    shutdown( tmp_link->session_socket, 2 );
                    close( tmp_link->file );
                    sessions.erase( tmp_link );
                    continue;
                }
            }
            else
                if( result == 0 )
                {
                    std::cout << "End of data, close session" << std::endl;
                    tmp_link = session;
                    session++;

                    shutdown( tmp_link->session_socket, 2 );
                    close( tmp_link->file );
                    sessions.erase( tmp_link );
                    continue;
                }
                else
                {
                    if( session->name_size == 0 )
                    {                           
                        session->name_size = strlen( msg->path );
                        char* path_to_file = new char[session->name_size];
                        strcpy( path_to_file, msg->path );
                        
                        std::cout << "msg->file '" << msg->path << "', msg->data '" << msg->data << "'" << std::endl;
                      
                        char* dir = new char[session->name_size];
                        memset( dir, 0, session->name_size );
                        char* path = dirname( path_to_file );

                        if( msg->path[0] == '/' )
                            chdir( "/" );
                        else
                            chdir( directory );
                        
                        char *ptr = strtok( path, "/" );
                        while( ptr != NULL )
                        {
                            strcat( dir, ptr );
                            strcat( dir, "/" );
                            
                            if( mkdir( dir, 0755 ) < 0 );
                                if( errno != EEXIST )
                                    notify( "mkdir()", errno );
                            
                            ptr = strtok (NULL, "/");
                        }
                        
                        if(( session->file = open( msg->path,  O_APPEND | O_RDWR | O_CREAT, 00644 )) < 0 )
                            notify( "open()", errno );
                        
                        memset( ascii, 0, sizeof( ascii ));
                        sprintf( ascii, "%d ", msg->data );
                        write( session->file, ascii, strlen( ascii ));
                        
                        delete( path );
                        delete( dir );
                        delete( path_to_file );
                    
                        continue;
                    }
                    else
                    {
                        std::cout << "msg->file '" << msg->path << "', msg->data '" << msg->data << "'" << std::endl;
                        
                        memset( ascii, 0, sizeof( ascii ));
                        sprintf( ascii, "%d ", msg->data );
                        write( session->file, ascii, strlen( ascii ));
                        
                        std::cout << std::endl;
                    }
                }
        }
        
        if( FD_ISSET( server_socket, &sock_d_set ))
        {
            while( true )
            {
                result = accept( server_socket, ( struct sockaddr* )NULL, NULL );
                if( result < 0 )
                {   
                    if( errno == EAGAIN || errno == EWOULDBLOCK )
                        break;
                    notify( "accept()", errno );
                }
                else
                {
                    if( fcntl( result, F_SETFL, O_NONBLOCK ) < 0 )
                        notify( "fcntl()", errno );
                    
                    session_data->session_socket = result;
                    session_data->name_size = 0;
                    
                    sessions.push_back( *session_data );
                    std::cout << "new connection, socket: " << session_data->session_socket << std::endl;
                }
            }
        }
    }
}

int recipient_t::start()
{
    int listen_socket, enable = 1;
    struct sockaddr_in listen_sockaddr;

    if(( listen_socket = socket( PF_INET, SOCK_STREAM, 0 )) < 0 )
        notify( "socket()", errno );
    
    setsockopt( listen_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof( int ));

    memset( &listen_sockaddr, 0, sizeof( listen_sockaddr ));
    listen_sockaddr.sin_family = PF_INET;
    listen_sockaddr.sin_port = htons( port );
    listen_sockaddr.sin_addr.s_addr = INADDR_ANY;

    if( bind( listen_socket, ( struct sockaddr* )&listen_sockaddr, sizeof( listen_sockaddr )) < 0 )
        notify( "bind()", errno );
    if( listen( listen_socket, 5 ) < 0 )
        notify( "listen()", errno );
    if( fcntl( listen_socket, F_SETFL, O_NONBLOCK ) < 0 )
        notify( "fcntl()", errno );

    return listen_socket;
}

void recipient_t::notify( const char *func, int code )
{
    std::cout << func << ": " << strerror( code ) << " code " << code << std::endl;
    // exit( 0 );
}
