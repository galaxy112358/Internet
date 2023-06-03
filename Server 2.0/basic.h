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

void getStart();//主要是关于Winsock的初始化

void theEnd();//同样关于Winsock

void printCurrentTime();

bool ESCPressed();
