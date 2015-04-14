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
			if(strChk != "-exit-")
			{
				bis.SendMessage(message, THIS_BIS);
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