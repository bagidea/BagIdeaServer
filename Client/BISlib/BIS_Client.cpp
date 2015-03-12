#include "BIS_Client.h"

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
	snprintf(sockMessage, sizeof(sockMessage), "%s\n%s", LOGIN_EVENT, username.c_str());
	send(sock_, sockMessage, strlen(sockMessage), IPPROTO_TCP);
	_username = username;
}

void BIS_Client::Disconnect()
{
	string str = DISCONNECT_EVENT;
	string username = _username;
	snprintf(sockMessage, sizeof(sockMessage), "%s\n%s", str.c_str(),username.c_str());
	send(sock_, sockMessage, strlen(sockMessage), IPPROTO_TCP);
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
	if((n = read(sock_, sockMessage, sizeof(sockMessage)-1)) > 0)
	{
		sockMessage[n] = 0;

		strVec = splitToVector(sockMessage, "\n");

		if(strVec[0] == CONNECT_COMPLETE)
		{
			param.status = CONNECT_COMPLETE;
		}
		else if(strVec[0] == LOGIN_EXIST)
		{
			param.status = LOGIN_EXIST;
		}
		else if(strVec[0] == LOGIN_COMPLETE)
		{
			strVec[1] = AllVecToString(strVec, 1, "\n");
			param.username = strVec[1];
			param.status = LOGIN_COMPLETE;
			if(param.username == _username)
				isLogin_ = true;
		}
		else if(strVec[0] == SEND_MESSAGE)
		{
			param.status = SEND_MESSAGE;
			param.username = strVec[1];
			strVec[2] = AllVecToString(strVec, 2, "\n");
			param.message = strVec[2];
		}
		else if(strVec[0] == DISCONNECT_EVENT)
		{
			param.status = DISCONNECT_EVENT;
			strVec[1] = AllVecToString(strVec, 1, "\n");
			param.username = strVec[1];
			if(param.username == _username)
			{
				close(sock_);
				return false;
			}
		}else{
			param.status = "";
		}
		return true;
	}else{
		param.status = SERVER_ERROR;
		close(sock_);
	}
	return false;
}

bool BIS_Client::SendMessage(string msg, int to)
{
	switch(to)
	{
		case THIS_BIS:
			snprintf(sockMessage, sizeof(sockMessage), "%s\n%s\n%s", SEND_MESSAGE, "THIS", msg.c_str());
			send(sock_, sockMessage, strlen(sockMessage), IPPROTO_TCP);
			break;
		case ALL_BIS:
			snprintf(sockMessage, sizeof(sockMessage), "%s\n%s\n%s", SEND_MESSAGE, "ALL", msg.c_str());
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