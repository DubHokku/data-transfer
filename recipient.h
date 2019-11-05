#include <iostream>
#include <fstream> 
#include <list>
#include <iterator>
#include <cstring>
#include <string>

#include <unistd.h>
#include <fcntl.h>
#include <libgen.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SIZE_O_PATH 127

class recipient_t
{
    public:
    recipient_t();
    ~recipient_t();
    
    void receive();
        
    private:
    int start();
    void notify( const char*, int );
    
    struct msg_t
    {
        int data;
        char path[SIZE_O_PATH];
    };
    
    struct session_t
    {
        int file;
        int session_socket;
        short name_size;
    };
    
    short port;
    int server_socket;
    msg_t *msg;
    session_t *session_data;
    
    std::list < session_t > sessions;
    std::list < session_t > :: iterator session;
    std::list < session_t > :: iterator tmp_link;
};
