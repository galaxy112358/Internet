#include "processor.h"

NineNineProcessor::NineNineProcessor()
{
	waitSeat = nullptr;
	uniN = 0; agentNum = 0;
	for (int i = 0; i < 2048; i++) agents[i] = nullptr;
}

NineNineProcessor::~NineNineProcessor()
{
	for (int i = 0; i < agentNum; i++) delete agents[i];
}

NineNineProcessor::NineNineAgent::NineNineAgent(int uniN, SOCKET skt, string N, string SN, int CN, int wR, int cT)
{
	uniqueNum = uniN;
	agentSocket = skt;
	name = N;
	skin = SN;
	colorNum = CN;
	winRate = wR;
	careerTotal = cT;
	score = 0;
	total = 0;
}

int NineNineProcessor::checkRequest(char* data)
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


	if (request == "PlayerInfo") return 0;//获取玩家信息
	else if (request == "Decision") return 1;//玩家游戏中的决策
	else if (request == "NextGame") return 2;//是否进行下一局
	else return -1;//若返回-1，则说明无需任何处理
}

int NineNineProcessor::sendPlayerInfo(const NineNineAgent* agent)
{
	string info = "{name:" + agent->name + ",skin:" + agent->skin
		+ ",colorNum:" + to_string(agent->colorNum) + ",winRate:" + to_string(agent->winRate)
		+ ",careerTotal:" + to_string(agent->careerTotal)
		+ ",uniqueNum:" + to_string(agent->uniqueNum) + "}";
	const char* p = info.c_str();
	if (send(agent->opponentSocket, p, 1024, 0) == SOCKET_ERROR)
	{
		cerr << "send error with code " << WSAGetLastError() << " !" << endl;
		return 1;
	}
	return 0;
}

int NineNineProcessor::getPlayerInfo(const char* data, NineNineAgent*& agent, SOCKET playerSocket)
{
	//cout << data << endl;
	string name;
	string skin;
	int colorNum = 0;
	int winRate = 0;
	int careerTotal = 0;

	int i = 0;
	bool flag = 0;
	string str, result;
	char temp[1024] = { 0 };
	for (i = 1; i < 1024; i++)
	{
		for (int j = i; j < 1024; j++)
		{
			if (data[j] == ':')
			{
				temp[j - i] = '\0';
				str = temp;
				i = j + 1;
				break;
			}
			temp[j - i] = data[j];
		}
		for (int j = i; j < 1024; j++)
		{
			if (data[j] == ',')
			{
				temp[j - i] = '\0';
				result = temp;
				i = j;
				break;
			}
			else if (data[j] == '}')
			{
				temp[j - i] = '\0';
				result = temp;
				i = j;
				flag = 1;
				break;
			}
			temp[j - i] = data[j];
		}
		if (str == "name") name = result;
		else if (str == "skin") skin = result;
		else if (str == "colorNum") colorNum = stoi(result);
		else if (str == "winRate") winRate = stoi(result);
		else if (str == "careerTotal") careerTotal = stoi(result);
		else
		{
			cerr << "NineNineProcessor::getPlayerInfo error !" << endl;
			system("pause");
		}
		if (flag) break;
	}

	//cout << name << " " << skin << " " << colorNum << " " << winRate << " " << careerTotal << endl;
	agent = new NineNineAgent(uniN, playerSocket, name, skin, colorNum, winRate, careerTotal);
	Sleep(500);
	string uni = "{uniqueNum:" + to_string(uniN) + "}";
	const char* p = uni.c_str();
	if (send(playerSocket, p, 1024, 0) == SOCKET_ERROR)
		cerr << "select1 error with code " << WSAGetLastError() << " !" << endl;
	agentNum++;
	uniN++;
	if (waitSeat == nullptr) waitSeat = agent;
	else
	{
		agent->opponentSocket = waitSeat->agentSocket;
		waitSeat->opponentSocket = agent->agentSocket;
		Sleep(500);
		sendPlayerInfo(agent);
		sendPlayerInfo(waitSeat);
		waitSeat = nullptr;
	}
	return 0;
}

int NineNineProcessor::sendDecision(const char* data)
{
	int x = 0, y = 0;
	int uniNum = 0;

	int i = 0;
	bool flag = 0;
	string str, result;
	char temp[1024] = { 0 };
	for (i = 1; i < 1024; i++)
	{
		for (int j = i; j < 1024; j++)
		{
			if (data[j] == ':')
			{
				temp[j - i] = '\0';
				str = temp;
				i = j + 1;
				break;
			}
			temp[j - i] = data[j];
		}
		for (int j = i; j < 1024; j++)
		{
			if (data[j] == ',')
			{
				temp[j - i] = '\0';
				result = temp;
				i = j;
				break;
			}
			else if (data[j] == '}')
			{
				temp[j - i] = '\0';
				result = temp;
				i = j;
				flag = 1;
				break;
			}
			temp[j - i] = data[j];
		}
		if (str == "x") x = stoi(result);
		else if (str == "y") y = stoi(result);
		else if (str == "uniqueNum") uniNum = stoi(result);
		else
		{
			cerr << "NineNineProcessor::sendDecision error !" << endl;
			system("pause");
		}
		if (flag) break;
	}

	for (int i = 0; i < agentNum; i++)
	{
		if (agents[i]->uniqueNum == uniNum)
		{
			string decision = "{x:" + to_string(x) + ",y:" + to_string(y) + "}";
			const char* p = decision.c_str();
			send(agents[i]->opponentSocket, p, 1024, 0);
			return 0;
		}
	}
	return 1;
}

POINT NineNineProcessor::processData(char* data, SOCKET tempSocket)
{//该函数返回值：x表示处理的数据类型，y表示处理情况（如是否异常）
	POINT result;
	switch (result.x = checkRequest(data))
	{
	case -1:
		result.y = 0;
		break;
	case 0:
		result.y = getPlayerInfo(data, agents[uniN], tempSocket);
		break;
	case 1:
		result.y = sendDecision(data);
		break;
	}
	return result;
}