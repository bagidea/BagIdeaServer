#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <vector>
#include <boost/algorithm/string.hpp>

#include "BIS_ServerObject.h"

#define CONNECT_COMPLETE "CONNECT_COMPLETE"
#define LOGIN_EVENT "LOGIN_EVENT"
#define LOGIN_EXIST "LOGIN_EXIST"
#define LOGIN_COMPLETE "LOGIN_COMPLETE"
#define SEND_MESSAGE "SEND_MESSAGE"
#define DISCONNECT_EVENT "DISCONNECT_EVENT"
#define THIS_BIS 0
#define ROOM_BIS 1
#define ALL_BIS 2

using namespace std;

class BIS_Server
{
public:
	struct Parameter
	{
		string status;
		string ip;
		string username;
		int* id;
	}param;

	bool SocketServer(int port);
	bool Bind();

	vector<string> splitToVector(string in_, string sp_);
	string AllVecToString(vector<string> vec, int index, string spStr = "");
	bool ReadMessage(int id);
	bool SendMessage(string msg, int to, int id, string room = "");

	bool hasConnect();
private:
	int connect_,n;
	int listen_;
	int* sock_;
	sockaddr_in server_, client_;
	socklen_t clientLength_;

	int port_;

	char sockMessage[256];
	vector<string> strVec;

	BIS_ServerObject serverData;
};