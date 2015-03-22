#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

#define CONNECT_COMPLETE "CONNECT_COMPLETE"
#define LOGIN_EVENT "LOGIN_EVENT"
#define LOGIN_EXIST "LOGIN_EXIST"
#define LOGIN_FAIL "LOGIN_FAIL"
#define LOGIN_COMPLETE "LOGIN_COMPLETE"
#define SEND_MESSAGE "SEND_MESSAGE"
#define CREATEROOM_EVENT "CREATEROOM_EVENT"
#define CREATEROOM_COMPLETE "CREATEROOM_COMPLETE"
#define CREATEROOM_FAIL "CREATEROOM_FAIL"
#define JOINROOM_EVENT "JOINROOM_EVENT"
#define JOINROOM_COMPLETE "JOINROOM_COMPLETE"
#define JOINROOM_FAIL "JOINROOM_FAIL"
#define LEAVEROOM_EVENT "LEAVEROOM_EVENT"
#define LEAVEROOM_COMPLETE "LEAVEROOM_COMPLETE"
#define LEAVEROOM_FAIL "LEAVEROOM_FAIL"
#define LOADROOM_EVENT "LOADROOM_EVENT"
#define LOADROOM_COMPLETE "LOADROOM_COMPLETE"
#define DESTROYROOM_EVENT "DESTROYROOM_EVENT"
#define DESTROYROOM_COMPLETE "DESTROYROOM_COMPLETE"
#define DESTROYROOM_FAIL "DESTROYROOM_FAIL"
#define DISCONNECT_EVENT "DISCONNECT_EVENT"
#define SERVER_ERROR "SERVER_ERROR"
#define NO_ROOM "|-(Not Have Room)-|"
#define THIS_BIS 0
#define ALL_BIS 1

using namespace std;

struct RoomObject
{
	string roomName;
	int maxUser;
	int countUser;
};

class BIS_Client
{
public:
	struct Parameter
	{
		string status;
		string username;
		string message;
		string room;
		int maxUser;
		int countUser;
		int countRoom;
		vector<string> roomNameList;
		vector<int> roomMaxUserList;
		vector<int> roomCountUserList;
	}param;

	BIS_Client();

	bool Socket(char* serverIP, int serverPort);
	bool Connect();
	bool Login(string username);
	void CreateRoom(string roomName, int maxUser);
	bool JoinRoom(string roomName);
	void LeaveRoom();
	void LoadRoom();
	void DestroyRoom(string roomName);
	void Disconnect();

	vector<string> splitToVector(string in_, string sp_);
	string AllVecToString(vector<string> vec, int index, string spStr = "");
	bool ReadMessage();
	bool SendMessage(string msg, int to);

	bool IsLogin();
	string GetUsername();
	string GetRoom();

	void (*ConnectComplete)();
	void (*LoginComplete)(string username);
	void (*LoginFail)(string status);
	void (*MessageUpdate)(string username, string message);
	void (*CreateRoomComplete)(string username, string roomName, int maxUser);
	void (*CreateRoomFail)();
	void (*JoinComplete)(string username, string roomName, int maxUser, int countUser);
	void (*JoinFail)();
	void (*LeaveComplete)(string username, int maxUser, int countUser);
	void (*LeaveFail)();
	void (*LoadRoomComplete)(int countRoom, vector<RoomObject*> roomObject);
	void (*DestroyRoomComplete)(string username, string roomName);
	void (*DestroyRoomFail)();
	void (*DisconnectComplete)(string username);
	void (*ServerError)();
private:
	int sock_,n;
	sockaddr_in server_;
	bool isLogin_;
	string _username;
	string _usernameTemp;
	string _room;
	string _roomTemp;

	char ip_[15];
	int port_;

	char sockMessage[1024];
	vector<string> strVec;
	vector<RoomObject*> roomObject_;
	RoomObject* roomOb;
};