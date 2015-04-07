#include "BIS_ServerObject.h"

bool BIS_ServerObject::CreateRoom(string roomName, int maxUser)
{	
	int i;
	if(roomName != "" && maxUser > 0)
	{
		if(nameRoomList_.size() != 0)
		{
			for(i = 0; i < nameRoomList_.size(); i++)
			{
				if(nameRoomList_[i] == roomName)
				{
					return false;
				}
			}
		}
		nameRoomList_.push_back(roomName);
		maxUserRoomList_.push_back(maxUser);
		countUserRoomList_.push_back(0);
	}else{
		return false;
	}
	return true;
}

bool BIS_ServerObject::JoinRoom(int id, string roomName)
{
	int i,a,maxUser, countUser;
	for(i = 0; i < nameRoomList_.size(); i++)
	{
		if(nameRoomList_[i] == roomName)
		{
			maxUser = maxUserRoomList_[i];
			countUser = countUserRoomList_[i];
			for(a = 0; a < idList_.size(); a++)
			{
				if(maxUser > countUser)
				{
					if(idList_[a] == id)
					{
						userRoomList_[a] = roomName;
						countUserRoomList_[i]++;
						return true;
					}
				}else{
					return false;
				}
			}
			return false;
		}
	}
	return false;
}

void BIS_ServerObject::LeaveRoom(int id)
{
	int i,a;
	for(i = 0; i < idList_.size(); i++)
	{
		if(idList_[i] == id)
		{
			if(userRoomList_[i] != "")
			{
				for(a = 0; a < nameRoomList_.size(); i++)
				{
					if(nameRoomList_[a] == userRoomList_[i])
					{
						userRoomList_[i] = "";
						countUserRoomList_[a]--;
						break;
					}
				}
			}
			break;
		}
	}
}

string BIS_ServerObject::LoadRoom()
{
	int i;
	char chr[10];
	string str = "";
	if(nameRoomList_.size() > 0)
	{
		for(i = 0; i < nameRoomList_.size(); i++)
		{
			str.append(nameRoomList_[i]);
			str.append("\n");
			snprintf(chr,sizeof(chr),"%d",maxUserRoomList_[i]);
			str.append(chr);
			str.append("\n");
			snprintf(chr,sizeof(chr),"%d",countUserRoomList_[i]);
			str.append(chr);
			if(i < nameRoomList_.size()-1)
				str.append("\n");
		}
	}else{
		return NO_ROOM;
	}
	return str;
}

string BIS_ServerObject::LoadAllUser()
{
	int i;
	string str = "";
	if(idList_.size() > 0)
	{
		for(i = 0; i < usernameList_.size(); i++)
		{
			str.append(usernameList_[i]);
			str.append("\n");
			if(userRoomList_[i] != "")
				str.append(userRoomList_[i]);
			else
				str.append("No Room");
			if(i < usernameList_.size()-1)
				str.append("\n");
		}
	}else{
		return NO_USER;
	}
	return str;
}

string BIS_ServerObject::LoadUserInRoom(string roomName)
{
	int i;
	string str = "";
	if(idList_.size() > 0)
	{
		if(roomName != "")
		{
			for(i = 0; i < nameRoomList_.size()+1; i++)
			{
				if(i == nameRoomList_.size())
					return NO_USER;
				if(roomName == nameRoomList_[i])
					break;
			}

			for(i = 0; i < usernameList_.size(); i++)
			{
				if(userRoomList_[i] == roomName)
				{
					str.append(usernameList_[i]);
					str.append("\n");
					if(roomName != "")
						str.append(userRoomList_[i]);
					else
						str.append("No Room");
					if(i < usernameList_.size()-1)
						str.append("\n");
				}
			}
		}else{
			return NO_USER;
		}
	}else{
		return NO_USER;
	}
	if(str == "")
		return NO_USER;
	return str;
}

bool BIS_ServerObject::DestroyRoom(string roomName)
{
	int i,a,num;
	for(i = 0; i < nameRoomList_.size(); i++)
	{
		if(nameRoomList_[i] == roomName)
		{
			num = countUserRoomList_[i];
			for(a = 0; a < userRoomList_.size(); a++)
			{
				if(userRoomList_[a] == nameRoomList_[i])
				{
					userRoomList_[a] = "";
					num--;
					if(num <= 0)
						break;
				}
			}

			nameRoomList_.erase(nameRoomList_.begin()+i);
			maxUserRoomList_.erase(maxUserRoomList_.begin()+i);
			countUserRoomList_.erase(countUserRoomList_.begin()+i);
			return true;
		}
	}
	return false;
}

bool BIS_ServerObject::CheckExistUsername(int id, string username)
{
	if(idList_.size() != 0)
	{
		int i;
		for(i = 0; i < idList_.size(); i++)
		{
			if(usernameList_[i] == username)
			{
				return false;
			}
		}
	}
	idList_.push_back(id);
	usernameList_.push_back(username);
	userRoomList_.push_back("");
	return true;
}

void BIS_ServerObject::DestroyUserFromID(int id)
{
	int i;
	for(i = 0; i < idList_.size(); i++)
	{
		if(idList_[i] == id)
		{
			idList_.erase(idList_.begin()+i);
			usernameList_.erase(usernameList_.begin()+i);
			userRoomList_.erase(userRoomList_.begin()+i);
			break;
		}
	}
}

vector<int> BIS_ServerObject::GetAllID()
{
	return idList_;
}

vector<int> BIS_ServerObject::GetUserRoomID(string room)
{
	int i;
	vector<int> vec;
	for(i = 0; i < idList_.size(); i++)
	{
		if(userRoomList_[i] == room)
		{
			vec.push_back(idList_[i]);
		}
	}
	return vec;
}

string BIS_ServerObject::GetUsernameFromID(int id)
{
	int i;
	bool chk = false;
	for(i = 0; i < idList_.size(); i++)
	{
		if(idList_[i] == id)
		{
			chk = true;
			break;
		}
	}

	if(chk)
		return usernameList_[i];
	else
		return "";
}

int BIS_ServerObject::GetIDFromUsername(string username)
{
	int i;
	for(i = 0; i < idList_.size(); i++)
	{
		if(usernameList_[i] == username)
		{
			break;
		}
	}
	return idList_[i];
}

string BIS_ServerObject::GetRoomFromID(int id)
{	
	int i;
	bool chk;
	for(i = 0; i < idList_.size(); i++)
	{
		if(idList_[i] == id)
		{
			chk = true;
			break;
		}
	}

	if(chk)
		return userRoomList_[i];
	else
		return "";
}

int BIS_ServerObject::GetMaxUser(string roomName)
{
	int i;
	for(i = 0; i < nameRoomList_.size(); i++)
	{
		if(nameRoomList_[i] == roomName)
		{
			break;
		}
	}
	return maxUserRoomList_[i];
}

int BIS_ServerObject::GetCountUser(string roomName)
{
	int i;
	for(i = 0; i < nameRoomList_.size(); i++)
	{
		if(nameRoomList_[i] == roomName)
		{
			break;
		}
	}
	return countUserRoomList_[i];
}