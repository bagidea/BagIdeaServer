#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <pthread.h>

#include "BISlib/BIS_Client.h"

using namespace std;
using namespace boost;

char serverIP[15];
int serverPort;

string strTemp;
char username[50];
char message[512];

BIS_Client bis;

bool Exit = false;

void* Input(void*)
{
	while(!Exit)
	{
		if(bis.IsLogin())
		{
			cin.getline(message, 50);

			string strChk = message;
			if(strChk != "exit")
			{
				bis.SendMessage(message, THIS_BIS);
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

				bis.Login(username);
			}
			else if(bis.param.status ==  LOGIN_EXIST)
			{
				cout << "     Login Exist." << endl;
				cout << "     Username: ";
					
				cin.getline(username, 50);

				bis.Login(username);
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