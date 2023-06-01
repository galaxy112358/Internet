#include "sever.h"

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

int Server::processData(const char* data)
{
    return 0;
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

                    cout << "New client connected: " << inet_ntoa(newClientAddr.sin_addr) << ":" << ntohs(newClientAddr.sin_port) << endl;

                    if (send(newClientSocket, "galaxy112358:'connect:1'", 25, 0) == SOCKET_ERROR)
                        cerr << "send error with code " << WSAGetLastError() << " !" << endl;
                }
                else
                {
                    char buffer[1024] = { 0 };
                    int recvResult = recv(tempSocket, buffer, sizeof(buffer), 0);
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
                    else processData(buffer);
                }
            }
        }
    }
    return 0;
}
