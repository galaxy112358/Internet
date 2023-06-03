#include "server.h"

Server::Server()
{
    getStandard();
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.S_un.S_addr = HOST_ADDR;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
        cerr << "create serverSocket error with code " << WSAGetLastError() << " !" << endl;
    if (bind(serverSocket, (LPSOCKADDR)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        cerr << "bind error with code " << WSAGetLastError() << " !" << endl;
}

Server::~Server()
{
    closesocket(serverSocket);
}

void Server::getStandard()
{
    cout << "输入端口号：";
    cin >> port;
    cout << "输入打印间隔时间（分）：";
    cin >> peroidic_printTime;
}

int Server::work()
{
    //开始监听
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
        cerr << "listen error with code " << WSAGetLastError() << " !" << endl;
    //正在监测的文件描述符集合
    fd_set clientSet;
    FD_ZERO(&clientSet);
    FD_SET(serverSocket, &clientSet);
    //select超时时间
    timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    //循环输出在线人数
    int t = 0;
    thread([&]() {
        while (true)
        {
            if (t == 0) {
                printCurrentTime();
                cout << "当前在线人数：" << clientSet.fd_count - 1 << endl;
            }
            t++;
            t = t % int(60 * peroidic_printTime);
            Sleep(1000);
        }
        }).detach();
        //检测退出
        bool flag = 1;
        thread([&]()
            {
                while (true)
                {
                    if (ESCPressed())
                    {
                        flag = 0;
                        break;
                    }
                    Sleep(50);
                }
            }).detach();
            //循环使用select函数判断套接字状态，对应进行相应处理
            while (flag)
            {
                fd_set copySet1 = clientSet;

                int result1 = select(0, &copySet1, nullptr, nullptr, &timeout);
                if (result1 == SOCKET_ERROR)
                {
                    printCurrentTime();
                    cerr << "select1 error with code " << WSAGetLastError() << " !" << endl;
                    break;
                }
                else if (result1 == 0)
                {
                    //cout << "select1 time out !" << endl;
                }
                else
                {
                    for (int i = 0; i < copySet1.fd_count; i++)
                    {
                        SOCKET tempSocket = copySet1.fd_array[i];

                        if (tempSocket == serverSocket)
                        {
                            sockaddr_in newClientAddr;
                            int addrSize = sizeof(sockaddr_in);
                            SOCKET newClientSocket = accept(serverSocket, (LPSOCKADDR)&newClientAddr, &addrSize);

                            FD_SET(newClientSocket, &clientSet);

                            cout << "New client connected: " << inet_ntoa(newClientAddr.sin_addr) << ": " << ntohs(newClientAddr.sin_port) << endl;

                            if (send(newClientSocket, "{ServerBroadcast:Connected Successfully !}", 43, 0) == SOCKET_ERROR)
                                cerr << "send error with code " << WSAGetLastError() << " !" << endl;
                        }
                        else
                        {
                            char buffer[1024] = { 0 };
                            int recvResult = recv(tempSocket, buffer, 1024, 0);
                            if (recvResult == SOCKET_ERROR)
                            {
                                //cerr << "recv error with code " << WSAGetLastError() << " !" << endl;
                                cout << "One client disconnected !" << endl;
                                closesocket(tempSocket);
                                FD_CLR(tempSocket, &clientSet);
                                continue;
                            }
                            else if (recvResult == 0)
                            {
                                cout << "One client disconnected !" << endl;
                                closesocket(tempSocket);
                                FD_CLR(tempSocket, &clientSet);
                                continue;
                            }
                            else processData(buffer, tempSocket);
                        }
                    }
                }
            }
            return 0;
}

int Server::checkRequest(char* data)
{
    int len = strlen(data), i = 0;
    char temp[1024] = { 0 };
    //获取“:”前的指令
    for (i = 1; i < 1024; i++)
    {
        if (data[i] == ':')
        {
            temp[i] = '\0';
            break;
        }
        temp[i - 1] = data[i];
    }
    string request = temp;
    //更改data：除去已经读取的上一层指令
    int n = i + 1;
    for (i = n; i < len - 1; i++) temp[i - n] = data[i];
    for (i = len - 1 - n; i < 1024; i++) temp[i] = 0;
    for (int j = 0; j < 1024; j++) data[j] = temp[j];


    if (request == "NineNine") return 1;//NineNine游戏的处理
    else return -1;//若返回-1，则说明无需任何处理
}

POINT Server::processData(char* data, SOCKET tempSocket)
{//该函数返回值：x表示处理的数据类型，y表示处理情况（如是否异常）
    POINT result;
    switch (result.x = checkRequest(data))
    {
    case -1:
        result.y = 0;
        break;
    case 1:
        result.y = ninenine.processData(data, tempSocket).y;
        break;
    }
    return result;
}
