#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <pthread.h>

#include "BISlib/BIS_Client.h"

using namespace std;
using namespace boost;

char serverIP[16];
int serverPort;

string strTemp;
string roomLeaveTemp;
char username[50];
char message[512];

BIS_Client bis;

bool Exit = false;

void* Input(void*)
{
	string room_name;
	int maxUser;

	while(!Exit)
	{
		if(bis.IsLogin())
		{
			cin.getline(message, 50);

			string strChk = message;
			if(strChk != "-exit-")
			{
				if(strChk == "-create-")
				{
					cout << "     Room Name: ";
					cin.getline(message, 50);
					room_name = message;
					cout << "     Max User: ";
					cin.getline(message, 50);
					maxUser = atoi(message);
					bis.CreateRoom(room_name, maxUser);
				}
				else if(strChk == "-room-")
				{
					bis.LoadRoom();
				}
				else if(strChk == "-myroom-")
				{
					if(bis.GetRoom() != "")
						cout << "     Your room: " << bis.GetRoom() << endl;
					else
						cout << "     You no room." << endl;
				}
				else if(strChk == "-join-")
				{
					cout << "     Room Name: ";
					cin.getline(message, 50);
					room_name = message;
					if(!bis.JoinRoom(room_name))
						cout << "     You has join room already. - " << bis.GetRoom() << endl;
				}
				else if(strChk == "-leave-")
				{
					roomLeaveTemp = bis.GetRoom();
					bis.LeaveRoom();
				}
				else if(strChk == "-allchat-")
				{
					cout << "     Enter Message: ";
					cin.getline(message, 50);
					bis.SendMessage(message, ALL_BIS);
				}
				else if(strChk == "-alluser-")
				{
					bis.LoadAllUser();
				}
				else if(strChk == "-userinroom-")
				{
					cout << "     Room Name: ";
					cin.getline(message, 50);
					room_name = message;
					bis.LoadUserInRoom(room_name);
				}else{
					bis.SendMessage(message, THIS_BIS);
				}
			}else{
				bis.Disconnect();
				Exit = true;
			}
		}
	}
}

void ConnectComplete()
{
	cout << "     Username: ";
	cin.getline(username, 50);

	if(!bis.Login(username))
		cout << "     You has login already. - " << bis.GetUsername() << endl;
}

void LoginComplete(string userName)
{
	if(userName != bis.GetUsername())
		cout << "     " << userName << " has Login." << endl;
	else
		cout << "     Login Complete." << endl << endl << "-------------------------------------------------------" << endl << endl;
}

void LoginFail(string status)
{
	if(status ==  LOGIN_EXIST || status ==  LOGIN_FAIL)
	{
		if(status ==  LOGIN_EXIST)
			cout << "     Login Exist." << endl;
		else if(status ==  LOGIN_FAIL)
			cout << "     Login Fail." << endl;

		cout << "     Username: ";
					
		cin.getline(username, 50);

		if(!bis.Login(username))
			cout << "     You has login already. - " << bis.GetUsername() << endl;
	}
}

void MessageUpdate(string userName, string msg)
{
	if(userName != bis.GetUsername())
		cout << userName << ": " << msg << endl;
}

void CreateRoomComplete(string userName, string room, int maxUser)
{
	if(userName != bis.GetUsername())
	{
		cout << "     " << userName << " has create room - " << room << " : MaxUser(" << maxUser << ")"<< endl;
	}else{
		cout << "     Create room complete. - " << room << " : MaxUser(" << maxUser << ")"<< endl;
		bis.JoinRoom(room);
	}
}

void CreateRoomFail()
{
	cout << "     Room Exist." << endl;
}

void JoinComplete(string userName, string room, int maxUser, int countUser)
{
	if(room == bis.GetRoom())
	{
		if(userName != bis.GetUsername())
			cout << "     " << userName << " has join room. [" << countUser << "/" << maxUser << "]" << endl;
		else
			cout << "     Join room complete. [" << countUser << "/" << maxUser << "]" << endl;
	}
}

void JoinFail()
{
	cout << "     Can't join room." << endl;
}

void LeaveComplete(string userName, int maxUser, int countUser)
{
	if(userName != bis.GetUsername())
	{
		cout << "     " << userName << " leave room." << endl;
	}else{
		cout << "     You has leave room." << endl;
		if(countUser == 0)
			bis.DestroyRoom(roomLeaveTemp);
	}
}

void LeaveFail()
{
	cout << "     You can't leave room. Because you no room." << endl;
}

void LoadRoomComplete(int countRoom, vector<RoomObject*> room)
{
	int i;
	if(countRoom > 0)
	{
		cout << "     Room Count: " << countRoom << endl;
		for(i = 0; i < countRoom; i++)
		{
			cout << "     - " << room[i]->roomName << " [" << room[i]->countUser << "/" << room[i]->maxUser << "]" << endl;
		}
	}else{
		cout << "     No room." << endl;
	}
}

void LoadUserComplete(int countUser, vector<UserObject*> user)
{
	int i;
	if(countUser > 0)
	{
		cout << "     User Count: " << countUser << endl;
		for(i = 0; i < countUser; i++)
		{
			cout << "     - " << user[i]->username << " [Room: ";
			if(user[i]->roomName != "")
				cout << user[i]->roomName << "]" << endl;
			else
				cout << "-]" << endl;
		}
	}else{
		cout << "     No user." << endl;
	}
}

void DestroyRoomComplete(string userName, string room)
{
	cout << "     " << userName << " destroy room. - " << room << endl;
}

void DestroyRoomFail()
{
	cout << "     Can't destroy room." << endl;
}

void DisconnectComplete(string userName)
{
	if(userName != bis.GetUsername())
	{
		cout << "     " << userName << " Disconnect." << endl;
	}else{
		cout << "     Disconnect." << endl;
		Exit = true;
	}
}

void ServerError()
{
	cout << "     Server Dead." << endl;
	Exit = true;
}

int main(int argc, char* argv[])
{
	char Port_[5];

	bis.ConnectComplete = &ConnectComplete;
	bis.LoginComplete = &LoginComplete;
	bis.LoginFail = &LoginFail;
	bis.MessageUpdate = &MessageUpdate;
	bis.CreateRoomComplete = &CreateRoomComplete;
	bis.CreateRoomFail = &CreateRoomFail;
	bis.JoinComplete = &JoinComplete;
	bis.JoinFail = &JoinFail;
	bis.LeaveComplete = &LeaveComplete;
	bis.LeaveFail = &LeaveFail;
	bis.LoadRoomComplete = &LoadRoomComplete;
	bis.LoadUserComplete = &LoadUserComplete;
	bis.DestroyRoomComplete = &DestroyRoomComplete;
	bis.DestroyRoomFail = &DestroyRoomFail;
	bis.DisconnectComplete = &DisconnectComplete;
	bis.ServerError = &ServerError;

	switch(argc)
	{
		case 3:
			strcpy(serverIP, argv[1]);
			serverPort = atoi(argv[2]);
			cout << "\nServer IP: " << serverIP << endl;
			cout << "Server Port: " << serverPort << endl << endl;
			break;
		case 2:
			strcpy(serverIP, argv[1]);
			cout << "\nServer IP: " << serverIP << endl;
			cout << "Server Port: ";
			cin.getline(Port_, 5);
			serverPort = atoi(Port_);
			cout << endl;
			break;
		default:
			cout << "\nServer IP: ";
			cin.getline(serverIP, 16);
			cout << "Server Port: ";
			cin.getline(Port_, 5);
			serverPort = atoi(Port_);
			cout << endl;
	}

	cout << "::::::::::::::::::::: SampleChat ::::::::::::::::::::::" << endl << endl;

	cout << "     Create Sock: ";
	if(!bis.Socket(serverIP, serverPort))
	{
		cout << "Fail." << endl;
		return 1;
	}
	cout << "Success." << endl;

	cout << "     Connect Sock: ";
	if(!bis.Connect())
	{
		cout << "Fail." << endl;
		return 1;
	}
	cout << "Success." << endl << endl;

	cout << ":::::::::::::::::::::: StartChat ::::::::::::::::::::::" << endl << endl;

	pthread_t Input_;
	if(pthread_create(&Input_, NULL, Input, NULL) < 0)
		cout << "SampleChat Error!" << endl;

	while(!Exit)
	{
		bis.ReadMessage();
	}

	cout << endl << ":::::::::::::::::::::: EndProgram :::::::::::::::::::::" << endl << endl;

	return 0;
}