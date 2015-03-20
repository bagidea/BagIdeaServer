#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <vector>
#include <boost/algorithm/string.hpp>

#include "BIS_ServerObject.h"

#define CONNECT_COMPLETE "CONNECT_COMPLETE"
#define LOGIN_EVENT "LOGIN_EVENT"
#define LOGIN_EXIST "LOGIN_EXIST"
#define LOGIN_COMPLETE "LOGIN_COMPLETE"
#define SEND_MESSAGE "SEND_MESSAGE"
#define CREATEROOM_EVENT "CREATEROOM_EVENT"
#define CREATEROOM_COMPLETE "CREATEROOM_COMPLETE"
#define CREATEROOM_FAIL "CREATEROOM_FAIL"
#define JOINROOM_EVENT "JOINROOM_EVENT"
#define JOINROOM_COMPLETE "JOINROOM_COMPLETE"
#define JOINROOM_FAIL "JOINROOM_FAIL"
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
		string room;
		int maxUser;
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