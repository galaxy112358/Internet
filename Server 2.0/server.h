#pragma once
#include "basic.h"
#include "processor.h"

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
    NineNineProcessor ninenine;
    void getStandard();//获取服务器调试输出间隔等信息
    int checkRequest(char* data);//确定请求种类
    POINT processData(char* data, SOCKET tempSocket);//处理客户端发过来的信息
public:
    int work();
};