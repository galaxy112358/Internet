#include "basic.h"

void getStart()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	// ��ʼ�� Winsock
}

void theEnd()
{
	// ���� Winsock
	WSACleanup();
}

void printCurrentTime()
{
	auto now = chrono::system_clock::now();
	time_t now_time = chrono::system_clock::to_time_t(now);
	cout << "��ǰʱ�䣺" << ctime(&now_time);
}

bool ESCPressed()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) return 1;
	else return 0;
}