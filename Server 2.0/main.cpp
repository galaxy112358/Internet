#include "server.h"

int main()
{
	getStart();
	Server server;
	server.work();
	theEnd();
	return 0;
}

/*
��������Ҫ��һ�����������ʱ��
1����processor.h�¼�һ����Ӧ����Processor��ʵ����ع��ܣ���Server�������ӦProcessor����
2���޸�Server��Ӧ�ĺ�����checkRequest��processData������Ӷ�Ӧ���ܣ���Ҫ������������ʵ�֣�

������Ϣ��ʽ��
{��Ϣ���:��Ϣ����}���� {NineNine:getPlayerRequest}
*/
