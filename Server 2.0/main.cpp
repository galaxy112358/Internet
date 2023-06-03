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
当服务器要跟一个新软件合作时：
1、在processor.h下加一个对应的新Processor，实现相关功能；在Server中添加相应Processor对象
2、修改Server相应的函数（checkRequest、processData），添加对应功能（主要功能在新类中实现）

交流信息格式：
{信息类别:信息内容}，如 {NineNine:getPlayerRequest}
*/
