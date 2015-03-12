#include "BIS_ServerObject.h"

bool BIS_ServerObject::CheckExistUsername(int id, string username)
{
	if(idList_.size() != 0)
	{
		for(int i = 0; i < idList_.size(); i++)
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