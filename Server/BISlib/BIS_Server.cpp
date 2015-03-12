#include "BIS_Server.h"

bool BIS_Server::SocketServer(int port)
{
	port_ = port;

	if((connect_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 || (listen_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		return false;

	return true;
}

bool BIS_Server::Bind()
{
	server_.sin_family = AF_INET;
	server_.sin_addr.s_addr = htonl(INADDR_ANY);
	server_.sin_port = htons(port_);

	if(bind(listen_, (struct sockaddr*)&server_, sizeof(server_)) < 0)
		return false;

	listen(listen_, 5);
	clientLength_ = sizeof(client_);

	return true;
}

vector<string> BIS_Server::splitToVector(string in_, string sp_)
{
	vector<string> out_;
	boost::split(out_, in_, boost::is_any_of(sp_), boost::token_compress_on);
	return out_;
}

string BIS_Server::AllVecToString(vector<string> vec, int index, string spStr)
{
	int i;
	string str = "";
	for(i = index; i < vec.size(); i++)
	{
		str.append(vec[i]);
		if(i < vec.size()-1)
			str.append(spStr);
	}
	return str;
}

bool BIS_Server::ReadMessage(int id)
{
	string str;
	if((n = read(id, sockMessage, sizeof(sockMessage)-1)) > 0)
	{
		char its[50];

		sockMessage[n] = 0;

		strVec = splitToVector(sockMessage, "\n");

		if(strVec[0] == LOGIN_EVENT)
		{
			strVec[1] = AllVecToString(strVec, 1, "\n");
			param.status = LOGIN_EVENT;
			param.username = strVec[1];

			if(!serverData.CheckExistUsername(id,param.username))
			{
				SendMessage(LOGIN_EXIST, THIS_BIS, id);
				return false;
			}

			str = LOGIN_COMPLETE;
			str.append("\n");
			str.append(param.username);
			SendMessage(str, ALL_BIS, id);
		}
		else if(strVec[0] == SEND_MESSAGE)
		{
			param.status = SEND_MESSAGE;
			string room;
			strVec[2] = AllVecToString(strVec, 2, "\n");
			str = SEND_MESSAGE;
			str.append("\n");
			str.append(serverData.GetUsernameFromID(id));
			str.append("\n");
			str.append(strVec[2]);
			if(strVec[1] == "THIS")
			{
				room = serverData.GetRoomFromID(id);
				SendMessage(str, ROOM_BIS, id, room);
			}
			else if(strVec[1] == "ALL")
			{
				//All SendMessage
			}
		}
		else if(strVec[0] == DISCONNECT_EVENT)
		{
			param.status = DISCONNECT_EVENT;
			strVec[1] = AllVecToString(strVec, 1, "\n");
			param.username = strVec[1];
			str = DISCONNECT_EVENT;
			str.append("\n");
			str.append(param.username);
			SendMessage(str, ALL_BIS, id);
			serverData.DestroyUserFromID(id);
			return false;
		}
		return true;
	}else{
		//SendMessage(DISCONNECT_EVENT, ALL_BIS, id);
		param.status = DISCONNECT_EVENT;
		param.username = serverData.GetUsernameFromID(id);
		str = DISCONNECT_EVENT;
		str.append("\n");
		str.append(param.username);
		SendMessage(str, ALL_BIS, id);
		serverData.DestroyUserFromID(id);
	}
	return false;
}

bool BIS_Server::SendMessage(string msg, int to, int id, string room)
{
	int i;
	vector<int> allID;
	snprintf(sockMessage, sizeof(sockMessage), "%s", msg.c_str());
	switch(to)
	{
		case THIS_BIS:
			send(id, sockMessage, strlen(sockMessage), IPPROTO_TCP);
			break;
		case ROOM_BIS:
			allID = serverData.GetUserRoomID(room);
			for(i = 0; i < allID.size(); i++)
			{
				send(allID[i], sockMessage, strlen(sockMessage), IPPROTO_TCP);
			}
			break;
		case ALL_BIS:
			allID = serverData.GetAllID();
			for(i = 0; i < allID.size(); i++)
			{
				send(allID[i], sockMessage, strlen(sockMessage), IPPROTO_TCP);
			}
			break;
		default:
			return false;
	}

	return true;
}

bool BIS_Server::hasConnect()
{
	if(connect_ = accept(listen_, (struct sockaddr*)&client_, &clientLength_))
	{
		param.ip = string(inet_ntoa(client_.sin_addr));
		sock_ = (int*)malloc(1);
		*sock_ = connect_;
		param.id = sock_;

		SendMessage(CONNECT_COMPLETE, THIS_BIS, *(int*)param.id);

		return true;
	}

	return false;
}