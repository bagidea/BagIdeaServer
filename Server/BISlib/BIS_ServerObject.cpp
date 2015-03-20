#include "BIS_ServerObject.h"

bool BIS_ServerObject::CreateRoom(string roomName, int maxUser)
{	
	int i;
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
	for(i = 0; i < idList_.size(); i++)
	{
		if(idList_[i] == id)
		{
			break;
		}
	}
	return usernameList_[i];
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
	for(i = 0; i < idList_.size(); i++)
	{
		if(idList_[i] == id)
		{
			break;
		}
	}
	return userRoomList_[i];
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