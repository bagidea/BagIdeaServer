#include <iostream>
#include <string>
#include <vector>

using namespace std;

class BIS_ServerObject
{
public:
	bool CheckExistUsername(int id, string username);
	void DestroyUserFromID(int id);
	vector<int> GetAllID();
	vector<int> GetUserRoomID(string room);
	string GetUsernameFromID(int id);
	int GetIDFromUsername(string username);
	string GetRoomFromID(int id);
private:
	vector<int> idList_;
	vector<string> usernameList_;
	vector<string> userRoomList_;
};