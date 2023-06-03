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
    void getStandard();//��ȡ��������������������Ϣ
    int checkRequest(char* data);//ȷ����������
    POINT processData(char* data, SOCKET tempSocket);//����ͻ��˷���������Ϣ
public:
    int work();
};