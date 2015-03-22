#include "BIS_Client.h"

BIS_Client::BIS_Client()
{
	isLogin_ = false;
	ConnectComplete = NULL;
	LoginComplete = NULL;
	LoginFail = NULL;
	MessageUpdate = NULL;
	CreateRoomComplete = NULL;
	CreateRoomFail = NULL;
	JoinComplete = NULL;
	JoinFail = NULL;
	LeaveComplete = NULL;
	LeaveFail = NULL;
	LoadRoomComplete = NULL;
	DestroyRoomComplete = NULL;
	DestroyRoomFail = NULL;
	DisconnectComplete = NULL;
	ServerError = NULL;
}

bool BIS_Client::Socket(char* serverIP, int serverPort)
{
	strcpy(ip_, serverIP);
	port_ = serverPort;

	if((sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		return false;

	server_.sin_family = AF_INET;
	server_.sin_port = htons(port_);

	if(inet_pton(AF_INET, serverIP, &server_.sin_addr) <= 0)
		return false;

	return true;
}

bool BIS_Client::Connect()
{
	if(connect(sock_, (struct sockaddr*)&server_, sizeof(server_)) < 0)
		return false;

	return true;
}

bool BIS_Client::Login(string username)
{
	if(isLogin_ == true && username != "")
		return false;

	_usernameTemp = username;
	snprintf(sockMessage, sizeof(sockMessage), "%s\n%s\f", LOGIN_EVENT, username.c_str());
	send(sock_, sockMessage, strlen(sockMessage), IPPROTO_TCP);
	return true;
}

void BIS_Client::CreateRoom(string roomName, int maxUser)
{
	if(isLogin_ == true)
	{
		snprintf(sockMessage, sizeof(sockMessage), "%s\n%s\n%d\f", CREATEROOM_EVENT, roomName.c_str(), maxUser);
		send(sock_, sockMessage, strlen(sockMessage), IPPROTO_TCP);
	}
}

bool BIS_Client::JoinRoom(string roomName)
{
	if(isLogin_  == false || _room != "")
		return false;

	_roomTemp = roomName;
	snprintf(sockMessage, sizeof(sockMessage), "%s\n%s\f", JOINROOM_EVENT, roomName.c_str());
	send(sock_, sockMessage, strlen(sockMessage), IPPROTO_TCP);
	return true;
}

void BIS_Client::LeaveRoom()
{
	if(isLogin_ == true)
	{
		snprintf(sockMessage, sizeof(sockMessage), "%s\f", LEAVEROOM_EVENT);
		send(sock_, sockMessage, strlen(sockMessage), IPPROTO_TCP);
	}
}

void BIS_Client::LoadRoom()
{
	if(isLogin_  == true)
	{
		snprintf(sockMessage, sizeof(sockMessage), "%s\f", LOADROOM_EVENT);
		send(sock_, sockMessage, strlen(sockMessage), IPPROTO_TCP);
	}
}

void BIS_Client::DestroyRoom(string roomName)
{
	if(isLogin_  == true)
	{
		snprintf(sockMessage, sizeof(sockMessage), "%s\n%s\f", DESTROYROOM_EVENT, roomName.c_str());
		send(sock_, sockMessage, strlen(sockMessage), IPPROTO_TCP);
	}
}

void BIS_Client::Disconnect()
{
	if(isLogin_  == true)
	{
		string str = DISCONNECT_EVENT;
		string username = _username;
		snprintf(sockMessage, sizeof(sockMessage), "%s\n%s\f", str.c_str(),username.c_str());
		send(sock_, sockMessage, strlen(sockMessage), IPPROTO_TCP);
	}
}

vector<string> BIS_Client::splitToVector(string in_, string sp_)
{
	vector<string> out_;
	boost::split(out_, in_, boost::is_any_of(sp_), boost::token_compress_on);
	return out_;
}

string BIS_Client::AllVecToString(vector<string> vec, int index, string spStr)
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

bool BIS_Client::ReadMessage()
{
	vector<string> strVecSP;
	int i,sp,a;
	if((n = read(sock_, sockMessage, sizeof(sockMessage)-1)) > 0)
	{
		sockMessage[n] = 0;

		strVecSP = splitToVector(sockMessage, "\f");
		for(sp = 0; sp < strVecSP.size()-1; sp++)
		{
			strVec = splitToVector(strVecSP[sp].c_str(), "\n");

			if(strVec[0] == CONNECT_COMPLETE)
			{
				param.status = CONNECT_COMPLETE;

				if(ConnectComplete != NULL)
					ConnectComplete();
			}
			else if(strVec[0] == LOGIN_EXIST)
			{
				param.status = LOGIN_EXIST;

				if(LoginFail != NULL)
					LoginFail(param.status);
			}
			else if(strVec[0] == LOGIN_COMPLETE)
			{
				param.status = LOGIN_COMPLETE;

				strVec[1] = AllVecToString(strVec, 1, "\n");
				param.username = strVec[1];
				if(param.username == _usernameTemp)
				{
					_username = param.username;
					isLogin_ = true;
				}

				if(LoginComplete != NULL)
					LoginComplete(param.username);
			}
			else if(strVec[0] == LOGIN_FAIL)
			{
				param.status = LOGIN_FAIL;

				if(LoginFail != NULL)
					LoginFail(param.status);
			}
			else if(strVec[0] == SEND_MESSAGE)
			{
				param.status = SEND_MESSAGE;

				param.username = strVec[1];
				strVec[2] = AllVecToString(strVec, 2, "\n");
				param.message = strVec[2];

				if(MessageUpdate != NULL)
					MessageUpdate(param.username, param.message);
			}
			else if(strVec[0] == CREATEROOM_COMPLETE)
			{
				param.status = CREATEROOM_COMPLETE;

				param.username = strVec[1];
				param.room = strVec[2];
				param.maxUser = atoi(strVec[3].c_str());

				if(CreateRoomComplete != NULL)
					CreateRoomComplete(param.username, param.room, param.maxUser);
			}
			else if(strVec[0] == CREATEROOM_FAIL)
			{
				param.status = CREATEROOM_FAIL;

				if(CreateRoomFail != NULL)
					CreateRoomFail();
			}
			else if(strVec[0] == JOINROOM_COMPLETE)
			{
				param.status = JOINROOM_COMPLETE;

				param.room = strVec[1];
				param.username = strVec[2];
				param.maxUser = atoi(strVec[3].c_str());
				param.countUser = atoi(strVec[4].c_str());
				if(param.username == _username && param.room == _roomTemp)
				{
					_room = _roomTemp;
				}

				if(JoinComplete != NULL)
					JoinComplete(param.username, param.room, param.maxUser, param.countUser);
			}
			else if(strVec[0] == JOINROOM_FAIL)
			{
				param.status = JOINROOM_FAIL;

				if(JoinFail != NULL)
					JoinFail();
			}
			else if(strVec[0] == LEAVEROOM_COMPLETE)
			{
				if(strVec[2] == _room)
				{
					param.status = LEAVEROOM_COMPLETE;

					param.username = strVec[1];
					param.maxUser = atoi(strVec[2].c_str());
					param.countUser = atoi(strVec[3].c_str());
					if(param.username == _username)
					{
						_room = "";
						_roomTemp = "";
					}
				}else{
					param.status = "";
				}

				if(LeaveComplete != NULL)
					LeaveComplete(param.username, param.maxUser, param.countUser);
			}
			else if(strVec[0] == LEAVEROOM_FAIL)
			{
				param.status = LEAVEROOM_FAIL;

					if(LeaveFail != NULL)
						LeaveFail();
			}
			else if(strVec[0] == LOADROOM_COMPLETE)
			{
				param.status = LOADROOM_COMPLETE;

				roomObject_.clear();
				if(strVec[1] != NO_ROOM)
				{
					strVec[1] = AllVecToString(strVec, 1, "\n");
					strVec = splitToVector(strVec[1].c_str(), "\n");

					for(i = 0; i < strVec.size(); i+=3)
					{
						roomOb = new RoomObject();
						roomOb->roomName = strVec[i];
						roomOb->maxUser = atoi(strVec[i+1].c_str());
						roomOb->countUser = atoi(strVec[i+2].c_str());

						roomObject_.push_back(roomOb);
					}

					param.countRoom = roomObject_.size();
				}else{
					param.countRoom = 0;
				}

				if(LoadRoomComplete != NULL)
					LoadRoomComplete(param.countRoom, roomObject_);
			}
			else if(strVec[0] == DESTROYROOM_COMPLETE)
			{
				param.status = DESTROYROOM_COMPLETE;

				param.room = strVec[1];
				param.username = strVec[2];

				if(DestroyRoomComplete != NULL)
					DestroyRoomComplete(param.username, param.room);
			}
			else if(strVec[0] == DESTROYROOM_FAIL)
			{
				param.status = DESTROYROOM_FAIL;

				if(DestroyRoomFail != NULL)
					DestroyRoomFail();
			}
			else if(strVec[0] == DISCONNECT_EVENT)
			{
				param.status = DISCONNECT_EVENT;

				strVec[1] = AllVecToString(strVec, 1, "\n");
				param.username = strVec[1];
				if(param.username == _username)
				{
					close(sock_);
					isLogin_ = false;
					_username = "";
				}

				if(DisconnectComplete != NULL)
					DisconnectComplete(param.username);
			}else{
				param.status = "";
			}
		}
		return true;
	}else{
		param.status = SERVER_ERROR;
		close(sock_);

		if(ServerError != NULL)
			ServerError();
	}
	return false;
}

bool BIS_Client::SendMessage(string msg, int to)
{
	switch(to)
	{
		case THIS_BIS:
			snprintf(sockMessage, sizeof(sockMessage), "%s\n%s\n%s\f", SEND_MESSAGE, "THIS", msg.c_str());
			send(sock_, sockMessage, strlen(sockMessage), IPPROTO_TCP);
			break;
		case ALL_BIS:
			snprintf(sockMessage, sizeof(sockMessage), "%s\n%s\n%s\f", SEND_MESSAGE, "ALL", msg.c_str());
			send(sock_, sockMessage, strlen(sockMessage), IPPROTO_TCP);
			break;
		default:
			return false;
	}
	return true;
}

bool BIS_Client::IsLogin()
{
	return isLogin_;
}

string BIS_Client::GetUsername()
{
	return _username;
}

string BIS_Client::GetRoom()
{
	return _room;
}