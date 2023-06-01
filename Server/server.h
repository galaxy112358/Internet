#pragma once
#include "basic.h"

class Server
{
#define HOST_ADDR INADDR_ANY
public:
    Server();
    ~Server();
private:
    short port;
    double peroidic_printTime;
    SOCKET serverSocket;
    sockaddr_in serverAddr;
    void getStandard();
    int processData(const char* data);
public:
    int work();
};
