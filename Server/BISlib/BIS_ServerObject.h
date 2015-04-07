#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#define NO_ROOM "|-(Not Have Room)-|"
#define NO_USER "|-(Not Have User)-|"

using namespace std;

class BIS_ServerObject
{
public:
	bool CreateRoom(string roomName, int maxUser);
	bool JoinRoom(int id, string roomName);
	void LeaveRoom(int id);
	string LoadRoom();
	string LoadAllUser();
	string LoadUserInRoom(string roomName);
	bool DestroyRoom(string roomName);
	bool CheckExistUsername(int id, string username);
	void DestroyUserFromID(int id);
	vector<int> GetAllID();
	vector<int> GetUserRoomID(string room);
	string GetUsernameFromID(int id);
	int GetIDFromUsername(string username);
	string GetRoomFromID(int id);
	int GetMaxUser(string roomName);
	int GetCountUser(string roomName);
private:
	vector<int> idList_;
	vector<string> usernameList_;
	vector<string> userRoomList_;

	vector<string> nameRoomList_;
	vector<int> maxUserRoomList_;
	vector<int> countUserRoomList_;
};