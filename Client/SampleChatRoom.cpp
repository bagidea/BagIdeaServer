#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <pthread.h>

#include "BISlib/BIS_Client.h"

using namespace std;
using namespace boost;

char serverIP[15];
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
						cout << "     You has join room already. - " << bis.GetRoom();
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

void Update(int argc_)
{
	int argc = argc_;
	int i;

	while(!Exit)
	{
		if(bis.ReadMessage())
		{
			if(bis.param.status ==  CONNECT_COMPLETE)
			{
				cout << "     Username: ";
				if(argc < 3)
				{
					cin.ignore();
				}
				cin.getline(username, 50);

				if(!bis.Login(username))
					cout << "     You has login already. - " << bis.GetUsername() << endl;
			}
			else if(bis.param.status ==  LOGIN_EXIST)
			{
				cout << "     Login Exist." << endl;
				cout << "     Username: ";
					
				cin.getline(username, 50);

				if(!bis.Login(username))
					cout << "     You has login already. - " << bis.GetUsername() << endl;
			}
			else if(bis.param.status ==  LOGIN_COMPLETE)
			{
				if(bis.param.username != bis.GetUsername())
					cout << "     " << bis.param.username << " has Login." << endl;
				else
					cout << "     Login Complete." << endl << endl << "-------------------------------------------------------" << endl << endl;
			}
			else if(bis.param.status ==  SEND_MESSAGE)
			{
				if(bis.param.username != bis.GetUsername())
					cout << bis.param.username << ": " << bis.param.message << endl;
			}
			else if(bis.param.status ==  CREATEROOM_COMPLETE)
			{
				if(bis.param.username != bis.GetUsername())
				{
					cout << "     " << bis.param.username << " has create room - " << bis.param.room << " : MaxUser(" << bis.param.maxUser << ")"<< endl;
				}else{
					cout << "     Create room complete. - " << bis.param.room << " : MaxUser(" << bis.param.maxUser << ")"<< endl;
					bis.JoinRoom(bis.param.room);
				}
			}
			else if(bis.param.status ==  CREATEROOM_FAIL)
			{
				cout << "     Room Exist." << endl;
			}
			else if(bis.param.status ==  JOINROOM_COMPLETE)
			{
				if(bis.param.room == bis.GetRoom())
				{
					if(bis.param.username != bis.GetUsername())
						cout << "     " << bis.param.username << " has join room. [" << bis.param.countUser << "/" << bis.param.maxUser << "]" << endl;
					else
						cout << "     Join room complete. [" << bis.param.countUser << "/" << bis.param.maxUser << "]" << endl;
				}
			}
			else if(bis.param.status ==  JOINROOM_FAIL)
			{
				cout << "     Can't join room." << endl;
			}
			else if(bis.param.status ==  LEAVEROOM_COMPLETE)
			{
				if(bis.param.username != bis.GetUsername())
				{
					cout << "     " << bis.param.username << " leave room." << endl;
				}else{
					cout << "     You has leave room." << endl;
					if(bis.param.countUser == 0)
						bis.DestroyRoom(roomLeaveTemp);
				}
			}
			else if(bis.param.status ==  LEAVEROOM_FAIL)
			{
				cout << "     You can't leave room. Because you no room." << endl;
			}
			else if(bis.param.status ==  LOADROOM_COMPLETE)
			{
				if(bis.param.countRoom > 0)
				{
					cout << "     Room Count: " << bis.param.countRoom << endl;
					for(i = 0; i < bis.param.countRoom; i++)
					{
						cout << "     - " << bis.param.roomNameList[i] << " [" << bis.param.roomCountUserList[i] << "/" << bis.param.roomMaxUserList[i] << "]" << endl;
					}
				}else{
					cout << "     No room." << endl;
				}
			}
			else if(bis.param.status ==  DESTROYROOM_COMPLETE)
			{
				cout << "     " << bis.param.username << " destroy room. - " << bis.param.room << endl;
			}
			else if(bis.param.status ==  DESTROYROOM_FAIL)
			{
				cout << "     Can't destroy room." << endl;
			}
			else if(bis.param.status ==  DISCONNECT_EVENT)
			{
				cout << "     " << bis.param.username << " Disconnect." << endl;
			}
		}else{
			if(bis.param.status ==  DISCONNECT_EVENT)
			{
				cout << "     Disconnect." << endl;
				Exit = true;
				break;
			}
			else if(bis.param.status ==  SERVER_ERROR)
			{
				cout << "     Server Dead." << endl;
				Exit = true;
				break;
			}
		}
	}
}

int main(int argc, char* argv[])
{
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
			cin >> serverPort;
			cout << endl;
			break;
		default:
			cout << "\nServer IP: ";
			cin.getline(serverIP, 15);
			cout << "Server Port: ";
			cin >> serverPort;
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

	Update(argc);

	cout << endl << ":::::::::::::::::::::: EndProgram :::::::::::::::::::::" << endl << endl;

	return 0;
}