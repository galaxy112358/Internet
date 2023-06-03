#pragma once
#include "basic.h"

class NineNineProcessor
{
public:
	NineNineProcessor();
	~NineNineProcessor();

	class NineNineAgent
	{
	public:
		SOCKET opponentSocket;
		SOCKET agentSocket;
		int uniqueNum;
		string name;
		string skin;
		int colorNum;
		int score, total;
		int winRate;
		int careerTotal;
		NineNineAgent(int uniN, SOCKET skt, string N = "Agent", string SN = " ", int CN = -1, int wR = 0, int cT = 0);
	};

	NineNineAgent* waitSeat;
	int agentNum, uniN;
	NineNineAgent* agents[2048];

	int checkRequest(char* data);
	int getPlayerInfo(const char* data, NineNineAgent*& agent, SOCKET playerSocket);
	int sendDecision(const char* data);
	int sendPlayerInfo(const NineNineAgent* agent);
	POINT processData(char* data, SOCKET tempSocket);
};