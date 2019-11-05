#include <iostream>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/stat.h>

#define SIZE_O_PATH 127

class sender_t
{
    public:
    sender_t();
    ~sender_t();
    
    struct msg_t
    {
        int data;
        char path[SIZE_O_PATH];
    };
    
    const char* path;
    const char* server;

    int start();
    int transfer( int*, int* );
    int stop( int* );
    
    private:
    short port;
    void notify( const char*, int );
};
