#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

#define CONNECT_COMPLETE "CONNECT_COMPLETE"
#define LOGIN_EVENT "LOGIN_EVENT"
#define LOGIN_EXIST "LOGIN_EXIST"
#define LOGIN_COMPLETE "LOGIN_COMPLETE"
#define SEND_MESSAGE "SEND_MESSAGE"
#define DISCONNECT_EVENT "DISCONNECT_EVENT"
#define SERVER_ERROR "SERVER_ERROR"
#define THIS_BIS 0
#define ALL_BIS 1

using namespace std;

class BIS_Client
{
public:
	struct Parameter
	{
		string status;
		string username;
		string message;
	}param;

	bool Socket(char* serverIP, int serverPort);
	bool Connect();
	bool Login(string username);
	void Disconnect();

	vector<string> splitToVector(string in_, string sp_);
	string AllVecToString(vector<string> vec, int index, string spStr = "");
	bool ReadMessage();
	bool SendMessage(string msg, int to);

	bool IsLogin();
	string GetUsername();
private:
	int sock_,n;
	sockaddr_in server_;
	bool isLogin_;
	string _username;

	char ip_[15];
	int port_;

	char sockMessage[512];
	vector<string> strVec;
};