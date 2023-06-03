#include "basic.h"

void getStart()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	// 初始化 Winsock
}

void theEnd()
{
	// 清理 Winsock
	WSACleanup();
}

void printCurrentTime()
{
	auto now = chrono::system_clock::now();
	time_t now_time = chrono::system_clock::to_time_t(now);
	cout << "当前时间：" << ctime(&now_time);
}

bool ESCPressed()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) return 1;
	else return 0;
}