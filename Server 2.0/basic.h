#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <Winsock2.h>  
#include <Windows.h>
#include <thread>
#include <ctime>
#include <chrono>
#include <mutex>
#include <condition_variable>
using namespace std;
#pragma comment(lib,"ws2_32.lib")

void getStart();//��Ҫ�ǹ���Winsock�ĳ�ʼ��

void theEnd();//ͬ������Winsock

void printCurrentTime();

bool ESCPressed();
