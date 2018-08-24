
#include <stdio.h>
#include <string>

using namespace std;

	extern bool StopConnect();
	extern bool CreateConnect(string IP, string Port);
	extern bool SendMsg(string str);
	extern string ReceiveMsg();